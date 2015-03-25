#ifndef _REMOTE_GYRO_H_
#define _REMOTE_GYRO_H_

#include <atomic>
#include <thread>

class RemoteGyro {
public:
  RemoteGyro();
  RemoteGyro(void *zmqCtx);
  ~RemoteGyro();

  void Reset();
  double GetAngle();

private:
  void Run();

  void *_zmqCtx;

  std::atomic<double> _angle;
  std::thread _thread;
};

#endif // _REMOTE_GYRO_H_
