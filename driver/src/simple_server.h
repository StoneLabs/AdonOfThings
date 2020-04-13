#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <WiFiNINA.h>
#include "secrets.h"

#define HTTP_GET_OFFSET_LEFT 4
#define HTTP_GET_OFFSET_RIGHT 9
#define HTTP_AUTH_OFFSET_LEFT 21
#define HTTP_AUTH_OFFSET_RIGHT 0

/* Very simple web server that allowed one client to connect at a time.
 * A HTTP1.1 request is read from the client and handler called for
 * a response body. Handler argument is the GET line.
 *
 * An auth password can be set by passing a WebServerAuth struct to
 * setAuth().
*/
class SimpleServer
{
private:
	WiFiServer server = NULL;
	String(*handler)(const String&);

public:
	bool auth = false;

public:
	SimpleServer(unsigned short port);
	~SimpleServer();

	void start();

	void setHandler(String(*f)(const String&));
	bool handle();
};

