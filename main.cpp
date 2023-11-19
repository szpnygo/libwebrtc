
#include "webrtc_app.h"
#include <iostream>

using namespace libwebrtc;

int main() {
  std::shared_ptr<WebRTCApp> app = std::make_shared<WebRTCApp>();
  auto device = app->getDesktopDevice();
  auto list = device->GetDesktopMediaList(DesktopType::kWindow);
  list->UpdateSourceList(true, true);
  auto total = list->GetSourceCount();
  for (int i = 0; i < total; i++) {
    auto source = list->GetSource(i);
    std::cout << source->name().c_string() << std::endl;
  }

  return 0;
}