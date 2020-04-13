#include "wifi_utils.h"

void wifi_print_status()
{
    Serial.print("Connected to SSID: ");
    Serial.println(WiFi.SSID());

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Signal strength (RSSI):");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}

bool wifi_connect(const char* ssid, const char* pass, int tries, int _delay)
{
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        while (true); // Critical failure
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        Serial.println("Please upgrade the firmware");
    }

    bool firstConnection = true;


    // Attempt to connect to Wifi network:
    for (int trie = 0; trie != tries; trie++)
    {
        Serial.print("Connecting to ");
        Serial.print(ssid);
        Serial.print("... [");
        Serial.print(trie);
        Serial.print("/");
        Serial.print(tries);
        Serial.println("]");

        // Connect to WPA/WPA2 network.
        if (WiFi.begin(ssid, pass) == WL_CONNECTED)
        {
            wifi_print_status();
            return true;
        }
        
        // Wait and try again
        delay(_delay);
    }

    Serial.println("Connection failed. Too many ties.");
    return false;
}