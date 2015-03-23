#ifndef _REMOTE_GYRO_H_
#define _REMOTE_GYRO_H_

#include <atomic>
#include <thread>

class RemoteGyro {
public:
  RemoteGyro();
  ~RemoteGyro();

  double GetAngle();

private:
  void Run();

  // ZeroMQ data
  void *_ctx;
  void *_sub;

  std::atomic<double> _angle;
  std::thread _thread;
};

#endif // _REMOTE_GYRO_H_
