#define DEBUG

#include <Arduino.h>
#include "wifi.h"
#include "alkomat.h"


void setup()
{
	Serial.begin(115200);
	WiFiManagement::initWifi();
	Alkomat::initScale();
}

void loop()
{
	Serial.println(Alkomat::readScale());
	delay(500);
}