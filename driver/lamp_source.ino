/*
 Name:		lamp_source.ino
 Created:	3/6/2020 9:46:20 PM
 Author:	Stone
*/

#include "src/secrets.h"
#include "src/wifi_utils.h"
#include "src/simple_server.h"

// Wifi data
const char ssid[] = SECRETS_SSID;       
const char pass[] = SECRETS_PASS;

// Wifi server
SimpleServer server(80);

// mode: false = manual; true = automatic;
bool mode = true;
const long pingRate = 60000;           // interval at which to check for internet connection (milliseconds)

// Establish wifi connection
void setup() 
{
    Serial.begin(9600);      // initialize serial communication
    delay(200);              // give monitor some time
    pinMode(4, OUTPUT);      // set the LED pin mode
    digitalWrite(4, LOW);    // set LED to RED

    // Connect to Wifi
    wifi_connect(SECRETS_SSID, SECRETS_PASS);

    // Start Server
    server.auth = true;
    server.setHandler(handleRequest);
    server.start();
    Serial.println("Server running on :80");
}

// the loop function runs over and over again until power down or reset
unsigned long lastPing = 0;
void loop() 
{
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Disconnect detected. Reconnecting.");
        setup();
    }

    // Check connection for display
    if (mode == true && millis() - lastPing >= pingRate)
    {
        lastPing = millis();

#ifdef _DEBUG
        Serial.print(millis());
        Serial.print(": Checking internet connection: ");
#endif
        if (WiFi.ping(IPAddress(8, 8, 8, 8), 128) >= 0) // -1 = no connection
        {
#ifdef _DEBUG
            Serial.println("CONNECTED");
#endif
            digitalWrite(4, HIGH); // GREEN
        }
        else
        {
#ifdef _DEBUG
            Serial.println("NO CONNECTION");
#endif
            digitalWrite(4, LOW); // RED
        }
    }

    // Handle web server clients
    if (server.handle())
    {
#ifdef _DEBUG
        Serial.print(millis());
        Serial.println(": Client Serverd!");
#endif
    }
}

String handleRequest(const String &s)
{
    String prefix = "";
    if (s == "/?l")
    {
        mode = false;
        digitalWrite(4, LOW); // RED
        prefix = "<h1>Current mode: MANUAL</h1><br/>Lamp set to red!";
    }
    else if (s == "/?h")
    {
        mode = false;
        digitalWrite(4, HIGH); // GREEN
        prefix = "<h1>Current mode: MANUAL</h1><br/>Lamp set to Green!";
    }
    else if (s == "/?a")
    {
        mode = true;
        prefix = "<h1>Current mode: PING-AUTOMATIC</h1><br/>Mode changed!";
    }
    else
    {
        if (mode)
            prefix = "<h1>Current mode: PING-AUTOMATIC</h1>";
        else
            prefix = "<h1>Current mode: MANUAL</h1>";
    }

    return prefix + "<br/><br/><a href=\"/?l\">Set to manual red!</a><br/><a href=\"/?h\">Set to manual green!</a><br/><a href=\"/?a\">Set to IP automatic!</a>";
}
