#pragma once

#include <atomic>
#include <thread>

#include <zmqpp.hpp>

class RemoteGyro {
public:
  RemoteGyro(zmqpp::context &ctx);
  ~RemoteGyro();

  void Reset();
  double GetAngle();
  double GetTemperature();

private:
  void Run();

  zmqpp::context &_ctx;

  std::atomic<double> _angle;
  std::atomic<double> _temperature;
  std::thread _thread;
};
