/**================================================================================================
 * ?                                           ABOUT
 * @author         :  Christopher Falkner
 * @email          :  christopher.falkner@web.de
 * @repo           :  -
 * @createdOn      :  10.05.22
 * @description    :  Alkomat (Longdrink Machine) Management and Control Class
 *================================================================================================**/

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "HX711.h"
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
// #include <ESP8266WebServer.h>
#include <ArduinoJson.h>



/**----------------------------------------------
 * *       CONSTANTS AND DEFINITIONS
 *---------------------------------------------**/ 
#define SERVO_OPENPOS 0
#define SERVO_CLOSEPOS 180
#define STEPS_PER_ML 175 / 2
#define DELAYTIME_DURING_FILL 10

#define HOSTNAME "Alkomat"
#define PORT 80


//todo  CHANGE THIS TO CLASS VARIABLE 
#define LOADCELL_DOUT_PIN D2
#define LOADCELL_SCK_PIN D3


#define DEBUG

 



#ifdef DEBUG
#define PRINT(s)            \
	{                       \
		Serial.print(s); \
	}
#else
#define PRINT(s)
#endif

enum valve_state_t
{
	closed,
	open,
};

enum drink_t
{
	drink1,
	drink2,
	drink3
};

/**--------------------------------------------
 *               VALVE CLASS
 *---------------------------------------------**/
class Valve
{
private:
	Servo _servoObject;
	uint8_t _pin = 0;
	valve_state_t _state;
	uint8_t id;
	drink_t _drink;

public:
	Valve();
	Valve(uint8_t pin);
	uint8_t setPin(uint8_t newPin);
	void open();
	void close();
	bool status();
	uint8_t setDrink(drink_t drink);
	uint8_t init(uint8_t pin, drink_t drink);
};

/**--------------------------------------------
 *               ALKOMAT CLASS
 *---------------------------------------------**/
class Alkomat
{
private:

	// ## WiFi Stuff ##
	uint16_t _port = 80;
    // ESP8266WebServer* _server;
    WiFiClient _client;
    String _header;
    String _message;

	void initScale();

public:
	Alkomat();
	HX711 _scale;
	Valve _valves[10];
	void addValve(uint8_t pin, drink_t drink);
	void changeDrink(Valve valve, drink_t newDrink);
	void pourDrink(uint8_t amount);
	void pourAmount(uint8_t amount, Valve valve);
	void calibrateScale(float knownWeight);
	void getStatus();

	/**======================
	 *    WiFi Stuff
	 *========================**/
	void initWifi();
    void startServer();
    void handleWiFi();

};


void endpointTest();
void handleNotFound();
void endpointStatus();
void endpointPour();
void endpointValve();
void endpointCalibrateScale();
void endpointScale();
