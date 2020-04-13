#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <WiFiNINA.h>

void wifi_print_status();

bool wifi_connect(const char* ssid, const char* pass, int tries, int delay);
inline bool wifi_connect(const char* ssid, const char* pass) { wifi_connect(ssid, pass, -1, 10000); }