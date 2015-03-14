#pragma once
#include <unordered_set>
#include <string>

class DumbDashboard {
public:
	DumbDashboard(void);
	~DumbDashboard(void);

	void SendDouble(std::string key, double value);
	void SendBool(std::string key, bool value);

private:
	void SendChannels(void);

	void *_ctx;
	void *_pub;

	std::unordered_set<std::string> _channels;
};
