#define DEBUG

#include <Arduino.h>
#include "wifi.h"
#include "alkomat.h"


void setup()
{
	Serial.begin(115200);
	WiFiManagement::initWifi();
	Alkomat::initScale();
	WiFiManagement::addToDebugLog("Starting...");
}

void loop()
{
	// Serial.println(Alkomat::readAverageScale());
	// WiFiManagement::addToDebugLog(String(Alkomat::readAverageScale()));


	delay(100);
	Alkomat::handle();
}