#include "RemoteLightRing.h"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

RemoteLightRing::RemoteLightRing(zmqpp::context &ctx)
  : _ctx(ctx),
    _socket(ctx, zmqpp::socket_type::req) {
  _socket.connect("tcp://10.23.58.16:31417");
}

RemoteLightRing::~RemoteLightRing() {
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

  _socket.send(s.GetString(), zmqpp::socket::dont_wait);
}
