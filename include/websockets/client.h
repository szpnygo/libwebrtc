#pragma once
#include <functional>
#include <ixwebsocket/IXWebSocket.h>
#include <memory>

typedef std::function<void(const std::string &message, const bool binary)>
    OnMessage;
typedef std::function<void(const std::string &error)> OnError;
typedef std::function<void()> OnOpen;
typedef std::function<void(const std::string &reason)> OnClose;
typedef std::function<void()> OnPing;
typedef std::function<void()> OnPong;

class WebSocketsClient {

public:
  WebSocketsClient();
  ~WebSocketsClient();

  void connect(const std::string &url);
  void close();
  void stop();

  ix::ReadyState getReadyState() const { return _webSocket->getReadyState(); };

  bool sendMessage(const std::string &data, bool binary = false);

  void onOpen(OnOpen onOpen) { _onOpen = onOpen; }
  void onError(OnError onError) { _onError = onError; }
  void onClose(OnClose onClose) { _onClose = onClose; }
  void onMessage(OnMessage onMessage) { _onMessage = onMessage; }
  void onPing(OnPing onPing) { _onPing = onPing; }
  void onPong(OnPong onPong) { _onPong = onPong; }

private:
  std::shared_ptr<ix::WebSocket> _webSocket;
  OnOpen _onOpen;
  OnError _onError;
  OnClose _onClose;
  OnMessage _onMessage;
  OnPing _onPing;
  OnPong _onPong;
};