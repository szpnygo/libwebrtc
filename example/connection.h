#pragma once
#include "client.h"
#include "webrtc_app.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#define OLIVEC_IMPLEMENTATION
#include "olive.h"
#include <chrono>

#define PI 3.14159265359

#define WIDTH 960
#define HEIGHT 960
#define BACKGROUND_COLOR 0xFF181818
#define GRID_COUNT 10
#define GRID_PAD 0.5 / GRID_COUNT
#define GRID_SIZE ((GRID_COUNT - 1) * GRID_PAD)
#define CIRCLE_RADIUS 5
#define Z_START 0.25
#define ABOBA_PADDING 50

using namespace libwebrtc;
using json = nlohmann::json;

typedef std::function<void()> OnConnectionSuccess;

class Connection : public RTCPeerConnectionObserver {
public:
  Connection(std::shared_ptr<WebRTCApp> app,
             std::shared_ptr<WebSocketsClient> client, std::string uid,
             std::string cid)
      : _app(app), _client(client), _uid(uid), _cid(cid) {

    std::cout << "create connection" << std::endl;

    RTCConfiguration config;
    config.ice_servers[0] =
        IceServer{"stun:coturn.tcodestudio.com:31000", "", ""};

    auto constraints = RTCMediaConstraints::Create();

    // create peer connection
    _pc = _app->getFactory()->Create(config, constraints);
    _pc->RegisterRTCPeerConnectionObserver(this);
  }

  ~Connection() {}

  scoped_refptr<RTCPeerConnection> get() { return _pc; }

  // init the connection
  void init() {
    auto stream = getStream();
    auto codecs =
        _app->getFactory()->GetRtpSenderCapabilities(RTCMediaType::VIDEO);
    auto codecsVector = codecs->codecs().std_vector();
    std::vector<std::string> order = {"video/AV1"};
    std::stable_sort(
        codecsVector.begin(), codecsVector.end(),
        [&order](const auto &a, const auto &b) {
          auto pos_a = std::distance(order.begin(),
                                     std::find(order.begin(), order.end(),
                                               a->mime_type().std_string()));
          auto pos_b = std::distance(order.begin(),
                                     std::find(order.begin(), order.end(),
                                               b->mime_type().std_string()));
          return pos_a < pos_b;
        });

    std::vector<string> std_vec = {stream->id()};
    for (const auto &item : stream->video_tracks().std_vector()) {
      auto sender = _pc->AddTrack(item, std_vec);
      if (sender) {
        for (const auto &transceiver : _pc->transceivers().std_vector()) {
          if (transceiver->sender()->id().std_string() ==
              sender->id().std_string()) {
            transceiver->SetCodecPreferences(codecsVector);
          }
        }
      }
    }

    std::cout << "init connection config success" << std::endl;
  }

  void createOffer() {
    auto constraints = RTCMediaConstraints::Create();
    constraints->AddMandatoryConstraint("OfferToReceiveAudio", "false");
    constraints->AddMandatoryConstraint("OfferToReceiveVideo", "false");

    _pc->CreateOffer(
        [this](const string sdp, const string type) {
          std::cout << "create offer success" << std::endl;
          onCreateOfferSuccess(sdp, type);
        },
        [](const char *error) {
          std::cout << "create offer failed: " << error << std::endl;
        },
        constraints);
  }

  void setAnswer(const json &data) {
    std::string sdp = data["sdp"];
    std::string type = data["type"];
    _pc->SetRemoteDescription(
        sdp, type, [this]() { std::cout << "set remote success" << std::endl; },
        [](const char *error) {});
  }

  void addCandidate(const json &data) {
    std::string sdpMid = data["sdpMid"];
    std::string candidate = data["candidate"];
    int sdpMLineIndex = data["sdpMLineIndex"];

    _pc->AddCandidate(sdpMid, sdpMLineIndex, candidate);
  }

  RTCPeerConnectionState getState() { return _state; }

  bool isConnected() {
    return _state == libwebrtc::RTCPeerConnectionStateConnected;
  }

  void onConnectionSuccess(OnConnectionSuccess success) { _success = success; }

  virtual void OnPeerConnectionState(RTCPeerConnectionState state) override {
    _state = state;
    if (state == libwebrtc::RTCPeerConnectionStateConnected) {
      if (_success) {
        _success();
      }
    }
  };

  virtual void
  OnIceCandidate(scoped_refptr<RTCIceCandidate> candidate) override {
    sendCandidateMessage(candidate);
  };

  virtual void OnIceConnectionState(RTCIceConnectionState state) override{};

  virtual void OnSignalingState(RTCSignalingState state) override{};

  virtual void OnIceGatheringState(RTCIceGatheringState state) override{};

  virtual void OnAddStream(scoped_refptr<RTCMediaStream> stream) override{};

  virtual void OnRemoveStream(scoped_refptr<RTCMediaStream> stream) override{};

  virtual void
  OnDataChannel(scoped_refptr<RTCDataChannel> data_channel) override{};

  virtual void OnRenegotiationNeeded() override{};

  virtual void OnTrack(scoped_refptr<RTCRtpTransceiver> transceiver) override{};

  virtual void OnAddTrack(vector<scoped_refptr<RTCMediaStream>> streams,
                          scoped_refptr<RTCRtpReceiver> receiver) override{};

  virtual void OnRemoveTrack(scoped_refptr<RTCRtpReceiver> receiver) override{};

protected:
  scoped_refptr<RTCVideoCapturer> _capturer;

  uint32_t pixels[WIDTH * HEIGHT];
  float angle = 0;
  const double frame_time = 1.0 / 30.0;

  Olivec_Canvas vc_render(float dt) {
    angle += 0.25 * PI * dt;

    Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);

    olivec_fill(oc, BACKGROUND_COLOR);
    for (int ix = 0; ix < GRID_COUNT; ++ix) {
      for (int iy = 0; iy < GRID_COUNT; ++iy) {
        for (int iz = 0; iz < GRID_COUNT; ++iz) {
          float x = ix * GRID_PAD - GRID_SIZE / 2;
          float y = iy * GRID_PAD - GRID_SIZE / 2;
          float z = Z_START + iz * GRID_PAD;

          float cx = 0.0;
          float cz = Z_START + GRID_SIZE / 2;

          float dx = x - cx;
          float dz = z - cz;

          float a = atan2f(dz, dx);
          float m = sqrtf(dx * dx + dz * dz);

          dx = cosf(a + angle) * m;
          dz = sinf(a + angle) * m;

          x = dx + cx;
          z = dz + cz;

          x /= z;
          y /= z;

          uint32_t r = ix * 255 / GRID_COUNT;
          uint32_t g = iy * 255 / GRID_COUNT;
          uint32_t b = iz * 255 / GRID_COUNT;
          uint32_t color =
              0xFF000000 | (r << (0 * 8)) | (g << (1 * 8)) | (b << (2 * 8));
          olivec_circle(oc, (x + 1) / 2 * WIDTH, (y + 1) / 2 * HEIGHT,
                        CIRCLE_RADIUS, color);
        }
      }
    }

    return oc;
  }

  // generate random frame as video source
  virtual void generateFrame() {
    std::srand(std::time(0));
    int frame_count = 0;
    const int FPS = 30;
    const std::chrono::milliseconds frameDuration(1000 / FPS);
    while (true) {
      auto frameStart = std::chrono::steady_clock::now();

      // scoped_refptr<libwebrtc::RTCVideoFrame> frame =
      //     GenerateRandomFrame(1920, 1080, frame_count);

      Olivec_Canvas oc = vc_render(frame_time);
      unsigned char *data = reinterpret_cast<unsigned char *>(oc.pixels);
      RTCVideoFrame::Type type = RTCVideoFrame::Type::kABGR;
      auto frame = RTCVideoFrame::Create(type, WIDTH, HEIGHT, data,
                                         WIDTH * HEIGHT * 4, WIDTH * 4);

      _capturer->GenerateFrame(frame);
      frame_count++;

      auto frameEnd = std::chrono::steady_clock::now();
      auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
          frameEnd - frameStart);

      if (frameTime < frameDuration) {
        std::this_thread::sleep_for(frameDuration - frameTime);
      }
    }
  }

private:
  scoped_refptr<RTCPeerConnection> _pc;
  std::shared_ptr<WebSocketsClient> _client;
  std::shared_ptr<WebRTCApp> _app;

  std::string _uid; // user id
  std::string _cid; // client id

  OnConnectionSuccess _success;

  RTCPeerConnectionState _state;
  std::thread timer_thread;

  // generate random frame as video source
  scoped_refptr<RTCVideoFrame> GenerateRandomFrame(int width, int height,
                                                   int frame_count) {
    // Create buffers for Y, U and V data.
    std::vector<uint8_t> data_y(width * height);
    std::vector<uint8_t> data_u(width * height / 4);
    std::vector<uint8_t> data_v(width * height / 4);

    // Fill Y, U and V buffers with random data.
    for (int i = 0; i < width * height; ++i) {
      data_y[i] = std::rand() % 256;
      if (i < width * height / 4) {
        data_u[i] = std::rand() % 256;
        data_v[i] = std::rand() % 256;
      }
    }

    // Add a growing white rectangle in the Y channel.
    int rect_size = frame_count % width;
    for (int y = 0; y < rect_size; ++y) {
      for (int x = 0; x < rect_size; ++x) {
        data_y[y * width + x] = 255;
      }
    }

    // Create RTCVideoFrame with random data.
    return RTCVideoFrame::Create(width, height, data_y.data(), width,
                                 data_u.data(), width / 2, data_v.data(),
                                 width / 2);
  }

  // get a video stream
  scoped_refptr<RTCMediaStream> getStream() {
    // create a stream
    auto stream = _app->getFactory()->CreateStream("stream_video");

    // create a capturer
    _capturer = _app->getVideoDevice()->CreateCapturer();

    // set the constraints
    auto constraints = RTCMediaConstraints::Create();

    // create a video source
    auto desktopSource = _app->getFactory()->CreateVideoSource(
        _capturer, "random_video_input", constraints);

    // create a video track
    auto tracker =
        _app->getFactory()->CreateVideoTrack(desktopSource, "tracker_video");

    // add the track to the stream
    stream->AddTrack(tracker);

    timer_thread = std::thread(&Connection::generateFrame, this);

    return stream;
  }

  void setSenderParameters() {
    if (_pc->senders().size() > 0) {
      auto sender = _pc->senders()[0];
      if (sender) {
        auto param = sender->parameters();
        auto encoding = param->encodings()[0];
        encoding->set_active(true);
        encoding->set_min_bitrate_bps(1000 * 1000 * 20); // 20M
        encoding->set_max_framerate(120);
        encoding->set_network_priority(RTCPriority::kHigh);
        std::vector<scoped_refptr<RTCRtpEncodingParameters>> new_encodings = {
            encoding};
        param->set_encodings(new_encodings);
        sender->set_parameters(param);
      }
    }
  }

  void onCreateOfferSuccess(const string sdp, const string type) {
    setSenderParameters();
    _pc->SetLocalDescription(
        sdp, type,
        [this, sdp]() {
          std::cout << "set local description success" << std::endl;
          sendOfferMessage(sdp.std_string());
        },
        [](const char *error) {
          std::cout << "set local description failed: " << error << std::endl;
        });
  }

  void sendOfferMessage(const std::string sdp) {
    json message;
    message["recipient"] = _cid;
    message["action"] = "OutgoingMessage";
    json offer;
    offer["action"] = "offer";
    offer["sdp"] = sdp;
    offer["type"] = "offer";
    message["content"] = offer.dump();

    auto result = _client->sendMessage(message.dump());
    if (!result) {
      std::cout << "send message failed: " << message.dump() << std::endl;
    } else {
      std::cout << "send offer success" << std::endl;
    }
  }

  void sendCandidateMessage(scoped_refptr<RTCIceCandidate> candidate) {
    json message;
    message["recipient"] = _cid;
    message["action"] = "OutgoingMessage";
    json data;
    data["action"] = "candidate";
    data["candidate"] = candidate->candidate().std_string();
    data["sdpMid"] = candidate->sdp_mid().std_string();
    data["sdpMLineIndex"] = candidate->sdp_mline_index();

    auto result = _client->sendMessage(message.dump());
    if (!result) {
      std::cout << "send message failed: " << message.dump() << std::endl;
    } else {
      std::cout << "send candidate success" << std::endl;
    }
  }
};