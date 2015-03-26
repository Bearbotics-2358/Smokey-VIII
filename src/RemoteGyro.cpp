#include "RemoteGyro.h"

#include <iostream>
#include <string.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

RemoteGyro::RemoteGyro(zmqpp::context &ctx)
  : _ctx(ctx),
    _angle(0.0),
    _temperature(0.0),
    _thread([this]() {Run();}) {
}

RemoteGyro::~RemoteGyro() {
}

void RemoteGyro::Reset() {
  // TODO: Implement
}

double RemoteGyro::GetAngle() {
  return _angle;
}

double RemoteGyro::GetTemperature() {
  return _temperature;
}

void RemoteGyro::Run() {
  try {
    zmqpp::socket socket(_ctx, zmqpp::socket_type::subscribe);
    socket.set(zmqpp::socket_option::subscribe, "gyro");

    socket.connect("tcp://10.23.58.16:31415");

    while (true) {
      // Receive the channel name (should be "gyro", since
      // that's the only string we subscribed to)
      std::string channel;
      socket.receive(channel);

      // Make sure there's another message part
      bool receive_more;
      socket.get(zmqpp::socket_option::receive_more, receive_more);
      if (!receive_more) {
        std::cout << "Received message only has one part" << std::endl;
        continue;
      }

      // Receive the second part, which should be the JSON
      // data from the gyro server
      std::string message;
      socket.receive(message);

      // Parse and store the data
      Document gyroData;
      gyroData.Parse(message.c_str());

      Value &angle = gyroData["angle"][2];
      _angle = angle.GetDouble();

      Value &temperature = gyroData["temp"];
      _temperature = temperature.GetDouble();
    }
  } catch (std::exception &ex) {
    std::cout << "Exception occured in RemoteGyro thread: " << ex.what() << std::endl;
  }
}
