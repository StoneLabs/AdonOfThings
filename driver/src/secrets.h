#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define SECRETS_SSID "wifi-name"
#define SECRETS_PASS "wifi-pass"
#define SECRETS_AUTH "http-auth"
