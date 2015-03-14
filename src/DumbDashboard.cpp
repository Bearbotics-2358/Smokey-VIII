#include "DumbDashboard.h"

#include <zmq.h>
#include <zmq_utils.h>
#include <zhelpers.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <Timer.h>

using namespace rapidjson;

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

	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.String("timestamp");
	writer.Double(Timer::GetFPGATimestamp());
	writer.String("data");
	writer.Double(value);
	writer.EndObject();

	s_sendmore(_pub, key.c_str());
	s_send(_pub, s.GetString());
}

void DumbDashboard::SendBool(std::string key, bool value) {
	_channels.emplace(key);
	SendChannels();

	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.String("timestamp");
	writer.Double(Timer::GetFPGATimestamp());
	writer.String("data");
	writer.Bool(value);
	writer.EndObject();

	s_sendmore(_pub, key.c_str());
	s_send(_pub, s.GetString());
}

void DumbDashboard::SendChannels(void) {
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.String("channels");
	writer.StartArray();

	std::unordered_set<std::string>::iterator it;
	for(it = _channels.begin(); it != _channels.end(); it++) {
		std::string channel = *it;
		writer.String(channel.c_str());
	}

	writer.EndArray();
	writer.EndObject();

	s_sendmore(_pub, "channels");
	s_send(_pub, s.GetString());
}
