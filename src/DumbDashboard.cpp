#include "DumbDashboard.h"

#include <zmq.h>
#include <zmq_utils.h>
#include "zhelpers.h"

DumbDashboard::DumbDashboard(void) {
	_ctx = zmq_ctx_new();
	_pub = zmq_socket(_ctx, ZMQ_PUB);
	zmq_bind(_pub, "tcp://*:12345");
}

DumbDashboard::~DumbDashboard(void) {
	zmq_close(_pub);
	zmq_ctx_destroy(_ctx);
}

void DumbDashboard::SendDouble(std::string key, double value) {
	_channels.emplace(key);
	SendChannels();

	s_sendmore(_pub, key.c_str());
	zmq_send(_pub, &value, sizeof(value), 0);
}

void DumbDashboard::SendChannels(void) {
	s_sendmore(_pub, "channels");
}
