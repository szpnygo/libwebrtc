#include "client.h"
#include "ixwebsocket/IXNetSystem.h"

WebSocketsClient::WebSocketsClient() {
  ix::initNetSystem();
  _webSocket = std::make_shared<ix::WebSocket>();
}

WebSocketsClient::~WebSocketsClient() {}

void WebSocketsClient::close() {
  _webSocket->disableAutomaticReconnection();
  _webSocket->close();
  ix::uninitNetSystem();
}

void WebSocketsClient::stop() {
  _webSocket->stop();
  ix::uninitNetSystem();
}

void WebSocketsClient::connect(const std::string &url) {
  _webSocket->setTLSOptions({});
  _webSocket->setUrl(url);
  _webSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg) {
    if (msg->type == ix::WebSocketMessageType::Message) {
      if (_onMessage) {
        _onMessage(msg->str, msg->binary);
      }
    } else if (msg->type == ix::WebSocketMessageType::Open) {
      if (_onOpen) {
        _onOpen();
      }
    } else if (msg->type == ix::WebSocketMessageType::Error) {
      if (_onError) {
        _onError(msg->errorInfo.reason);
      }
    } else if (msg->type == ix::WebSocketMessageType::Close) {
      if (_onClose) {
        _onClose(msg->closeInfo.reason);
      }
    } else if (msg->type == ix::WebSocketMessageType::Ping) {
      if (_onPing) {
        _onPing();
      }
    } else if (msg->type == ix::WebSocketMessageType::Pong) {
      if (_onPong) {
        _onPong();
      }
    }
  });
  _webSocket->start();
}

bool WebSocketsClient::sendMessage(const std::string &data, bool binary) {
  auto result = _webSocket->send(data, binary);
  return result.success;
}