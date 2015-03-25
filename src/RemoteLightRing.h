#pragma once

class RemoteLightRing {
public:
  RemoteLightRing();
  RemoteLightRing(void *zmqCtx);
  ~RemoteLightRing();

  void SetColor(int device, int r, int g, int b);

private:
  void *_zmqCtx;
  void *_zmqSocket;
};
