#include "RemoteLightRing.h"

#include <iostream>

#include <zmq.h>
#include <zhelpers.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

RemoteLightRing::RemoteLightRing()
  : RemoteLightRing(zmq_ctx_new()) {
}

RemoteLightRing::RemoteLightRing(void *zmqCtx)
  : _zmqCtx(zmqCtx) {
  _zmqSocket = zmq_socket(_zmqCtx, ZMQ_REQ);
  if (_zmqSocket == NULL) {
    std::cout << "RemoteLightRing: Failed to open request socket" << std::endl;
    return;
  }
  int result = zmq_connect(_zmqSocket, "tcp://10.23.58.16:31417");
  if (result != 0) {
    std::cout << "RemoteLightRing: Failed to connect to Pi" << std::endl;
    return;
  }
}

RemoteLightRing::~RemoteLightRing() {
  zmq_close(_zmqSocket);
}

void RemoteLightRing::SetColor(int device, int r, int g, int b) {
  StringBuffer s;
  Writer<StringBuffer> writer(s);

  writer.StartObject();
  writer.String("command");
  writer.String("setRGB");
  writer.String("data");
  writer.StartObject();
  writer.String("device");
  writer.Int(device);
  writer.String("red");
  writer.Int(r);
  writer.String("green");
  writer.Int(g);
  writer.String("blue");
  writer.Int(b);
  writer.EndObject();
  writer.EndObject();

  s_send(_zmqSocket, s.GetString());
}
