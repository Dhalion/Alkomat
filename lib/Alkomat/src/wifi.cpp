#include <ESPAsync_WiFiManager.h>               //https://github.com/khoih-prog/ESPAsync_WiFiManager


#include "wifi.h"
#include "debug.h"
#include "alkomat.h"

using namespace WiFiManagement;


AsyncWebServer server(80);
DynamicJsonDocument logStream(8192);


void WiFiManagement::initWifi(){
    AsyncWebServer  _ws(80);
    DNSServer       _dnss;
    ESPAsync_WiFiManager wifiManager(&_ws, &_dnss);
    DebugPrintln("Starting WiFi autoconnect...");
    wifiManager.autoConnect(AP_NAME);
    WiFi.hostname(HOSTNAME);
    DebugPrintln("Wifi connected.");
    WiFiManagement::initWebServer();
}


void WiFiManagement::initWebServer(){
    DebugPrintln("Starting web server...");

    /**==============================================================
     **                     SERVER ROUTES                          */

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });


    server.on("/scale", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<30> doc;
        String json;
        doc["weight"] = String(Alkomat::readAverageScale());
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });


    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<30> doc;
        String json;
        doc["status"] = "Healthy";
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });


    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
        String json;
        serializeJson(logStream, json);
        request->send(200, "application/json", json);
        // Flush the log when transmitted
        logStream.clear();
    });


    server.on("/calibration", HTTP_ANY, [](AsyncWebServerRequest *request){

        for (int i = 0; i < request->params(); i++) {
            AsyncWebParameter *p = request->getParam(i);
            String p_name = p->name();

            if (p_name == "mass") {
                addToDebugLog("Calibrating with mass of " + p->value() + "g");
                Serial.println("Calibrating with mass of " + p->value() + "g");
                // Alkomat::calibrateScale(p->value().toInt());  //! Not possible with AsyncWebServer

                //* Push Comand to Queue instead 
                Alkomat::addCommandToQueue({CommandType::calibrate, p->value().toInt()});
                DebugPrintln("# Added Calibration command to Queue.");
            }
        }


        request->send(200);

    });

    /**
     ** =============================================================*/


    server.onNotFound(notFound);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
    DebugPrintln("WebServer Started.");
}



void WiFiManagement::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}



void WiFiManagement::addToDebugLog(String log){
    JsonObject logObj = logStream.createNestedObject();
    logObj["timestamp"] = millis();
    logObj["message"] = log;
}

void WiFiManagement::debugLogHousekeeping(){
    // This is a hard limit
    if (logStream.size() > 1000) {
        logStream.clear();
    }
}
