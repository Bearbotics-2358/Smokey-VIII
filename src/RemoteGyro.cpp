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
    std::cout << "The Pi is a lie! zmq_connect returned "
              << result << std::endl;
    return;
  }

  while (true) {
    char *chan = s_recv(sub);
    if (chan == NULL) {
      std::cout << "Failed to get gyro message" << std::endl;
      continue;
    }

    // Figure out if the second part of the message is here
    int more = 0;
    size_t more_size = sizeof(more);
    zmq_getsockopt(sub, ZMQ_RCVMORE, &more, &more_size);
    if (!more) {
      std::cout << "Received message only has one part" << std::endl;
      free(chan);
      continue;
    }

    char *data = s_recv(sub);
    if (data == NULL) {
      std::cout << "Couldn't get the second part of the message" << std::endl;
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
