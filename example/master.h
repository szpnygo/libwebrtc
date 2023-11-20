#pragma once
#include "client.h"
#include "connection.h"
#include "webrtc_app.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

using namespace libwebrtc;
using json = nlohmann::json;

template <typename T> class Master {
  constexpr static char kServer[] =
      "wss://signal.webrtc.tcode.ltd/base?auth=gowebrtc-signal";

public:
  Master() {
    _client = std::make_shared<WebSocketsClient>();
    _app = std::make_shared<WebRTCApp>();
  };

  ~Master(){};

  void start() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this] { return _isStop; });
  }

  void stop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _isStop = true;
    _cv.notify_all();
  }

  void bindUser(std::string rid) {
    _uid = rid;
    _client->onOpen([this]() { this->connected(); });
    _client->onMessage([this](const std::string &message, const bool binary) {
      this->onMessage(message);
    });
    _client->connect(kServer);
  }

private:
  std::string _uid;
  std::shared_ptr<WebSocketsClient> _client;
  std::shared_ptr<WebRTCApp> _app;

  std::mutex _mutex;
  std::condition_variable _cv;
  bool _isStop = false;

  std::unordered_map<std::string, std::shared_ptr<T>> _connections;

  void connected() {
    // send bind user message
    json message;
    message["action"] = "BindUser";
    message["name"] = _uid;

    sendMessage(message);
  }

  bool sendMessage(const json &message) {
    auto result = _client->sendMessage(message.dump());
    if (!result) {
      std::cout << "send message failed: " << message.dump() << std::endl;
    }
    return result;
  }

  void onMessage(const std::string &message) {
    auto result = json::parse(message);
    std::string action = result["action"];
    std::cout << "receive message: " << message << std::endl;
    std::cout << "receive action: " << action << std::endl;
    if (action == "BindSuccess") {
      std::cout << "bind success " << _uid << std::endl;
    }
    if (action == "BindFail") {
      std::cout << "bind fail" << std::endl;
    }

    // receive message
    if (action == "IncomingMessage") {
      std::string content = result["content"];
      auto body = json::parse(content);
      if (body["action"] == "go") {
        std::string sender = result["sender"];
        std::cout << "go create webrtc server" << sender << std::endl;
        // client want to connect
        auto conn = std::make_shared<T>(_app, _client, _uid, sender);

        _connections[result["sender"]] = conn;
        conn->init();
        conn->createOffer();
      }
      if (body["action"] == "answer") {
        _connections[result["sender"]]->setAnswer(body);
      }
      if (body["action"] == "candidate") {
        _connections[result["sender"]]->addCandidate(body);
      }
    }
  }
};