#include "DumbDashboard.h"

#include <zmq.h>
#include <zmq_utils.h>
#include <zhelpers.h>

DumbDashboard::DumbDashboard(void) {
	_ctx = zmq_ctx_new();
	_pub = zmq_socket(_ctx, ZMQ_PUB);
	zmq_bind(_pub, "tcp://*:12345");
}

DumbDashboard::~DumbDashboard(void) {
	zmq_close(_pub);
	zmq_ctx_destroy(_ctx);
}

void DumbDashboard::SendDouble(const char *key, double value) {
	char buffer[32];
	sprintf(buffer, "%.4f", value);

	s_sendmore(_pub, key);
	s_send(_pub, buffer);
}
