#include "client.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using json = nlohmann::json;

class WebSocketsClientTest : public ::testing::Test {
protected:
  WebSocketsClient client;
};

TEST_F(WebSocketsClientTest, Connect) {
  std::atomic_bool successCalled(false);
  client.onOpen([&successCalled]() { successCalled = true; });
  client.connect("wss://signal.webrtc.tcode.ltd/base?auth=gowebrtc-signal");

  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(successCalled);
}

TEST_F(WebSocketsClientTest, Close) {
  std::atomic_bool successCalled(false);
  client.onOpen([this]() { client.close(); });
  client.onClose(
      [&successCalled](const std::string &reason) { successCalled = true; });
  client.connect("wss://signal.webrtc.tcode.ltd/base?auth=gowebrtc-signal");

  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(successCalled);
}
