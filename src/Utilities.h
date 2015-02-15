#pragma once
#include <WPILib.h>

double GetSmartDashboardNumber(std::string key, double defaultValue) {
	double result = defaultValue;
	try {
		result = SmartDashboard::GetNumber(key);
	} catch(...) {
		SmartDashboard::PutNumber(key, defaultValue);
	}
	return result;
}
