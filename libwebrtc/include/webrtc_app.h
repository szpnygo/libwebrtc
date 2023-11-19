#ifndef LIB_WEBRTC_APP_HXX
#define LIB_WEBRTC_APP_HXX

#include <unordered_map>

#include "base/scoped_ref_ptr.h"
#include "libwebrtc.h"
#include "rtc_desktop_device.h"
#include "rtc_desktop_media_list.h"
#include "rtc_peerconnection.h"
#include "rtc_peerconnection_factory.h"
#include "rtc_types.h"
#include "rtc_video_device.h"
#include "rtc_video_frame.h"

namespace libwebrtc {

class WebRTCApp {
 public:
  WebRTCApp() {
    LibWebRTC::Initialize();

    _factory = LibWebRTC::CreateRTCPeerConnectionFactory();
    _desktop_device = _factory->GetDesktopDevice();
    _video_device = _factory->GetVideoDevice();
  };
  ~WebRTCApp() { LibWebRTC::Terminate(); };

  scoped_refptr<RTCPeerConnectionFactory> getFactory() { return _factory; }
  scoped_refptr<RTCDesktopDevice> getDesktopDevice() { return _desktop_device; }
  scoped_refptr<RTCVideoDevice> getVideoDevice() { return _video_device; }

 protected:
  scoped_refptr<RTCPeerConnectionFactory> _factory;
  scoped_refptr<RTCDesktopDevice> _desktop_device;
  scoped_refptr<RTCVideoDevice> _video_device;
};
}  // namespace libwebrtc

#endif  // LIB_WEBRTC_APP_HXX