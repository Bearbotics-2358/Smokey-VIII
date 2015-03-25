#pragma once

#include <atomic>
#include <thread>

class RemoteGyro {
public:
  RemoteGyro();
  RemoteGyro(void *zmqCtx);
  ~RemoteGyro();

  void Reset();
  double GetAngle();
  double GetTemperature();

private:
  void Run();

  void *_zmqCtx;

  std::atomic<double> _angle;
  std::atomic<double> _temperature;
  std::thread _thread;
};
