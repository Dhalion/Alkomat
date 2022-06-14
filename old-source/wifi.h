/**================================================================================================
 * ?                                           ABOUT
 * @author         :  Christopher Falkner
 * @email          :  christopher.falkner@web.de
 * @repo           :  -
 * @createdOn      :  16.05.22
 * @description    :  Alkomat WiFi and API Handler
 *================================================================================================**/

#pragma once

#include <Arduino.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "alkomat.h"

/**----------------------------------------------
 * *       CONSTANTS AND DEFINITIONS
 *---------------------------------------------**/
#define HOSTNAME "Alkomat"
#define PORT 80

class WiFiHandler
{
public:
    WiFiHandler();
    void initWifi();
    void startServer();
    void handleWiFi();

private:
    uint16_t _port = 80;
    ESP8266WebServer *_server;
    WiFiClient _client;
    String _header;
    String _message;
    Alkomat _alkomat;
};

void endpointTest();
void handleNotFound();
void endpointStatus();
void endpointPour();
