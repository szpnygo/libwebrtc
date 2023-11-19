#include "rtc_video_frame_impl.h"

#include "api/video/i420_buffer.h"
#include "libyuv/convert.h"
#include "libyuv/convert_argb.h"
#include "libyuv/convert_from.h"
#include "libyuv/convert_from_argb.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"

namespace libwebrtc {

VideoFrameBufferImpl::VideoFrameBufferImpl(
    rtc::scoped_refptr<webrtc::VideoFrameBuffer> frame_buffer)
    : buffer_(frame_buffer) {}

VideoFrameBufferImpl::VideoFrameBufferImpl(
    rtc::scoped_refptr<webrtc::I420Buffer> frame_buffer)
    : buffer_(frame_buffer) {}

VideoFrameBufferImpl::~VideoFrameBufferImpl() {}

scoped_refptr<RTCVideoFrame> VideoFrameBufferImpl::Copy() {
  scoped_refptr<VideoFrameBufferImpl> frame =
      scoped_refptr<VideoFrameBufferImpl>(
          new RefCountedObject<VideoFrameBufferImpl>(buffer_));
  return frame;
}

int VideoFrameBufferImpl::width() const { return buffer_->width(); }

int VideoFrameBufferImpl::height() const { return buffer_->height(); }

const uint8_t* VideoFrameBufferImpl::DataY() const {
  return buffer_->GetI420()->DataY();
}

const uint8_t* VideoFrameBufferImpl::DataU() const {
  return buffer_->GetI420()->DataU();
}

const uint8_t* VideoFrameBufferImpl::DataV() const {
  return buffer_->GetI420()->DataV();
}

int VideoFrameBufferImpl::StrideY() const {
  return buffer_->GetI420()->StrideY();
}

int VideoFrameBufferImpl::StrideU() const {
  return buffer_->GetI420()->StrideU();
}

int VideoFrameBufferImpl::StrideV() const {
  return buffer_->GetI420()->StrideV();
}

int VideoFrameBufferImpl::ConvertToARGB(Type type, uint8_t* dst_buffer,
                                        int dst_stride, int dest_width,
                                        int dest_height) {
  rtc::scoped_refptr<webrtc::I420Buffer> i420 =
      webrtc::I420Buffer::Rotate(*buffer_.get(), rotation_);

  rtc::scoped_refptr<webrtc::I420Buffer> dest =
      webrtc::I420Buffer::Create(dest_width, dest_height);

  dest->ScaleFrom(*i420.get());
  int buf_size = dest->width() * dest->height() * (32 >> 3);
  switch (type) {
    case libwebrtc::RTCVideoFrame::Type::kARGB:
      libyuv::I420ToARGB(dest->DataY(), dest->StrideY(), dest->DataU(),
                         dest->StrideU(), dest->DataV(), dest->StrideV(),
                         dst_buffer, dest->width() * 32 / 8, dest->width(),
                         dest->height());
      break;
    case libwebrtc::RTCVideoFrame::Type::kBGRA:
      libyuv::I420ToBGRA(dest->DataY(), dest->StrideY(), dest->DataU(),
                         dest->StrideU(), dest->DataV(), dest->StrideV(),
                         dst_buffer, dest->width() * 32 / 8, dest->width(),
                         dest->height());
      break;
    case libwebrtc::RTCVideoFrame::Type::kABGR:
      libyuv::I420ToABGR(dest->DataY(), dest->StrideY(), dest->DataU(),
                         dest->StrideU(), dest->DataV(), dest->StrideV(),
                         dst_buffer, dest->width() * 32 / 8, dest->width(),
                         dest->height());
      break;
    case libwebrtc::RTCVideoFrame::Type::kRGBA:
      libyuv::I420ToRGBA(dest->DataY(), dest->StrideY(), dest->DataU(),
                         dest->StrideU(), dest->DataV(), dest->StrideV(),
                         dst_buffer, dest->width() * 32 / 8, dest->width(),
                         dest->height());
      break;
    default:
      break;
  }
  return buf_size;
}

libwebrtc::RTCVideoFrame::VideoRotation VideoFrameBufferImpl::rotation() {
  switch (rotation_) {
    case webrtc::kVideoRotation_0:
      return RTCVideoFrame::kVideoRotation_0;
    case webrtc::kVideoRotation_90:
      return RTCVideoFrame::kVideoRotation_90;
    case webrtc::kVideoRotation_180:
      return RTCVideoFrame::kVideoRotation_180;
    case webrtc::kVideoRotation_270:
      return RTCVideoFrame::kVideoRotation_270;
    default:
      break;
  }
  return RTCVideoFrame::kVideoRotation_0;
}

scoped_refptr<RTCVideoFrame> RTCVideoFrame::Create(int width, int height,
                                                   const uint8_t* buffer,
                                                   int length) {
  int stride_y = width;
  int stride_uv = (width + 1) / 2;

  int size_y = stride_y * height;
  int size_u = stride_uv * height / 2;
  // int size_v = size_u;

  RTC_DCHECK(length == (width * height * 3) / 2);

  const uint8_t* data_y = buffer;
  const uint8_t* data_u = buffer + size_y;
  const uint8_t* data_v = buffer + size_y + size_u;

  rtc::scoped_refptr<webrtc::I420Buffer> i420_buffer = webrtc::I420Buffer::Copy(
      width, height, data_y, stride_y, data_u, stride_uv, data_v, stride_uv);

  scoped_refptr<VideoFrameBufferImpl> frame =
      scoped_refptr<VideoFrameBufferImpl>(
          new RefCountedObject<VideoFrameBufferImpl>(i420_buffer));
  return frame;
}

scoped_refptr<RTCVideoFrame> RTCVideoFrame::Create(
    int width, int height, const uint8_t* data_y, int stride_y,
    const uint8_t* data_u, int stride_u, const uint8_t* data_v, int stride_v) {
  rtc::scoped_refptr<webrtc::I420Buffer> i420_buffer = webrtc::I420Buffer::Copy(
      width, height, data_y, stride_y, data_u, stride_u, data_v, stride_v);

  scoped_refptr<VideoFrameBufferImpl> frame =
      scoped_refptr<VideoFrameBufferImpl>(
          new RefCountedObject<VideoFrameBufferImpl>(i420_buffer));
  return frame;
}

scoped_refptr<RTCVideoFrame> RTCVideoFrame::Create(Type type, int width,
                                                   int height,
                                                   const uint8_t* buffer,
                                                   int length, int stride) {
  int pixelBit = 4;
  if (type == Type::kRGB24) {
    pixelBit = 3;
  }

  RTC_DCHECK(length == (width * height * pixelBit));

  rtc::scoped_refptr<webrtc::I420Buffer> i420_buffer =
      webrtc::I420Buffer::Create(width, height);

  // 创建空的Y、U、V通道
  std::unique_ptr<uint8_t[]> y_plane(new uint8_t[width * height]);
  std::unique_ptr<uint8_t[]> u_plane(
      new uint8_t[(width + 1) / 2 * (height + 1) / 2]);
  std::unique_ptr<uint8_t[]> v_plane(
      new uint8_t[(width + 1) / 2 * (height + 1) / 2]);

  switch (type) {
    case libwebrtc::RTCVideoFrame::Type::kARGB:
      libyuv::ARGBToI420(buffer, stride, y_plane.get(), width, u_plane.get(),
                         (width + 1) / 2, v_plane.get(), (width + 1) / 2, width,
                         height);
      break;
    case libwebrtc::RTCVideoFrame::Type::kBGRA:
      libyuv::BGRAToI420(buffer, stride, y_plane.get(), width, u_plane.get(),
                         (width + 1) / 2, v_plane.get(), (width + 1) / 2, width,
                         height);
      break;
    case libwebrtc::RTCVideoFrame::Type::kABGR:
      libyuv::ABGRToI420(buffer, stride, y_plane.get(), width, u_plane.get(),
                         (width + 1) / 2, v_plane.get(), (width + 1) / 2, width,
                         height);
      break;
    case libwebrtc::RTCVideoFrame::Type::kRGBA:
      libyuv::RGBAToI420(buffer, stride, y_plane.get(), width, u_plane.get(),
                         (width + 1) / 2, v_plane.get(), (width + 1) / 2, width,
                         height);
      break;
    case libwebrtc::RTCVideoFrame::Type::kRGB24:
      libyuv::RGB24ToI420(buffer, stride, y_plane.get(), width, u_plane.get(),
                          (width + 1) / 2, v_plane.get(), (width + 1) / 2,
                          width, height);
      break;
    default:
      break;
  }

  auto newBuffer =
      i420_buffer->Copy(width, height, y_plane.get(), width, u_plane.get(),
                        (width + 1) / 2, v_plane.get(), (width + 1) / 2);

  // 创建RTCVideoFrame对象
  scoped_refptr<VideoFrameBufferImpl> frame =
      scoped_refptr<VideoFrameBufferImpl>(
          new RefCountedObject<VideoFrameBufferImpl>(newBuffer));

  return frame;
}

}  // namespace libwebrtc
