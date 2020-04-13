#include "simple_server.h"

SimpleServer::SimpleServer(unsigned short port)
{
	this->server = WiFiServer(port);
}
SimpleServer::~SimpleServer()
{
}

void SimpleServer::start()
{
	this->server.begin();
}

void SimpleServer::setHandler(String(*f)(const String&))
{
	this->handler = f;
}

bool SimpleServer::handle()
{
	WiFiClient client = this->server.available();

	if (!client)
		return false;

	String request = "";

	// Read request bytes
	while (client.connected())  // connected
		if (client.available()) // ready to be read from
		{
			char read = client.read();
			if (read != '\r')
				request += read;

			if (request.endsWith("\n\n")) // end of HTTP request
				break;
		}

	// Example header line -> "Authorization: Basic base64=="
	int auth_start = request.indexOf("Authorization: Basic ");
	int auth_end = request.indexOf("\n", auth_start);

	// Find GET Header line
	int GET_start = request.indexOf("GET ");
	int GET_end = request.indexOf("\n", GET_start);

	if (GET_start >= 0 && GET_end >= 0)
	{
		if (!this->auth || (auth_start >= 0 && auth_end >= 0 && request.substring(auth_start + HTTP_AUTH_OFFSET_LEFT, auth_end - HTTP_AUTH_OFFSET_RIGHT) == SECRETS_AUTH))
		{
			// Send Response Header
			client.println("HTTP/1.1 200 OK");
			client.println("Content-type:text/html");
			client.println();

			// Send Response Body
			client.println(this->handler(request.substring(GET_start + HTTP_GET_OFFSET_LEFT, GET_end - HTTP_GET_OFFSET_RIGHT)));
		}
		else
		{
			client.println("HTTP/1.1 401 Unauthorized");
			client.println("WWW-Authenticate: Basic realm=\"restricted access\"");
			client.println("");
			client.println("401 Unauthorized");
		}
	}
	else
	{
		client.println("HTTP/1.1 400 Bad Request");
		client.println("");
		client.println("400 Bad Request");
	}

	// Disconnect client to finish transaction
	client.stop();
	return true;
}