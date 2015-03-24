#include "RemoteGyro.h"

#include <iostream>
#include <string.h>

#include <zmq.h>
#include <zhelpers.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

RemoteGyro::RemoteGyro()
  : _angle(0.0),
    _thread([this]() {Run();}) {
}

RemoteGyro::~RemoteGyro() {
}

double RemoteGyro::GetAngle() {
  return _angle;
}

void RemoteGyro::Run() {
  void *ctx = zmq_ctx_new();
  void *sub = zmq_socket(ctx, ZMQ_SUB);

  const char *channel = "gyro";
  int result = zmq_setsockopt(sub, ZMQ_SUBSCRIBE,
      channel, strlen(channel));

  result = zmq_connect(sub, "tcp://10.23.58.16:31415");
  if (result != 0) {
    std::cout << "Couldn't find any Pi! Here's what we found instead: "
              << result << std::endl;
    return;
  }

  while (true) {
    char *chan = s_recv(sub);
    if (chan == NULL) {
      continue;
    }

    // Figure out if the second part of the message is here
    int more = 0;
    size_t more_size = sizeof(more);
    zmq_getsockopt(sub, ZMQ_RCVMORE, &more, &more_size);
    if (!more) {
      std::cout << "Received message only has one part - skipping." << std::endl;
      free(chan);
      continue;
    }

    char *data = s_recv(sub);
    if (data == NULL) {
      free(chan);
      continue;
    }

    Document gyroData;
    gyroData.Parse(data);

    Value &angle = gyroData["angle"][2];
    _angle = angle.GetDouble();

    free(chan);
    free(data);
  }
}
