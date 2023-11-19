#ifndef LIB_WEBRTC_VIDEO_DEVICE_IMPL_HXX
#define LIB_WEBRTC_VIDEO_DEVICE_IMPL_HXX

#include <memory>

#include "modules/video_capture/video_capture.h"
#include "rtc_base/thread.h"
#include "rtc_video_device.h"
#include "rtc_video_frame_impl.h"
#include "src/internal/vcm_capturer.h"
#include "src/internal/video_capturer.h"

namespace libwebrtc {

class RTCVideoCapturerImpl : public RTCVideoCapturer {
 public:
  RTCVideoCapturerImpl(
      std::shared_ptr<webrtc::internal::VideoCapturer> video_capturer)
      : video_capturer_(video_capturer) {}
  std::shared_ptr<webrtc::internal::VideoCapturer> video_capturer() {
    return video_capturer_;
  }

  bool StartCapture() override {
    return video_capturer_ != nullptr && video_capturer_->StartCapture();
  }

  bool CaptureStarted() override {
    return video_capturer_ != nullptr && video_capturer_->CaptureStarted();
  }

  void StopCapture() override {
    if (video_capturer_ != nullptr) video_capturer_->StopCapture();
  }

  void GenerateFrame(scoped_refptr<RTCVideoFrame> frame) override {
    if (video_capturer_ != nullptr) {
      VideoFrameBufferImpl* impl =
          static_cast<VideoFrameBufferImpl*>(frame.get());

      auto newFrame = webrtc::VideoFrame::Builder()
                          .set_video_frame_buffer(impl->buffer())
                          .set_rotation(static_cast<webrtc::VideoRotation>(
                              impl->rotation()))
                          .set_timestamp_us(rtc::TimeMicros())
                          .build();

      video_capturer_->GenerateFrame(newFrame);
    }
  }

 private:
  std::shared_ptr<webrtc::internal::VideoCapturer> video_capturer_;
};

class RTCVideoDeviceImpl : public RTCVideoDevice {
 public:
  RTCVideoDeviceImpl(rtc::Thread* signaling_thread, rtc::Thread* worker_thread);

 public:
  uint32_t NumberOfDevices() override;

  int32_t GetDeviceName(uint32_t deviceNumber, char* deviceNameUTF8,
                        uint32_t deviceNameLength, char* deviceUniqueIdUTF8,
                        uint32_t deviceUniqueIdUTF8Length,
                        char* productUniqueIdUTF8 = 0,
                        uint32_t productUniqueIdUTF8Length = 0) override;

  scoped_refptr<RTCVideoCapturer> Create(const char* name, uint32_t index,
                                         size_t width, size_t height,
                                         size_t target_fps) override;

  scoped_refptr<RTCVideoCapturer> CreateCapturer() override;

 private:
  std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> device_info_;
  rtc::Thread* signaling_thread_ = nullptr;
  rtc::Thread* worker_thread_ = nullptr;
};

}  // namespace libwebrtc

#endif  // LIB_WEBRTC_VIDEO_DEVICE_IMPL_HXX
