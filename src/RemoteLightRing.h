#pragma once

#include <zmqpp.hpp>

class RemoteLightRing {
public:
  RemoteLightRing(zmqpp::context &ctx);
  ~RemoteLightRing();

  void SetColor(int device, int r, int g, int b);

private:
  zmqpp::context &_ctx;
  zmqpp::socket _socket;
};
