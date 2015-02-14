#pragma once

class DumbDashboard {
public:
	DumbDashboard(void);
	~DumbDashboard(void);

	void SendDouble(const char *key, double value);

private:
	void *_ctx;
	void *_pub;
};
