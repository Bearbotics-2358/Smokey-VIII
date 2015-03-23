#include "RemoteGyro.h"

#include <iostream>

#include <zmq.h>

RemoteGyro::RemoteGyro()
  : _angle(0.0),
    _thread([this]() {Run();}) {
}

RemoteGyro::~RemoteGyro() {
  _thread.join(); // Necessary? Advisable?!
  zmq_ctx_destroy(_ctx);
}

double RemoteGyro::GetAngle() {
  return _angle;
}

void RemoteGyro::Run() {
  _ctx = zmq_ctx_new();
  _sub = zmq_socket(_ctx, ZMQ_SUB);
  int result = zmq_connect(_sub, "tcp://10.23.58.16:31415");
  if (result != 0) {
    std::cout << "Couldn't find any Pi! Here's what we found instead: "
              << result << std::endl;
    return;
  }

  while (true) {
  }
}
