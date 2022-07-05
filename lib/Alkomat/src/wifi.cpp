#include <ESPAsync_WiFiManager.h>               //https://github.com/khoih-prog/ESPAsync_WiFiManager


#include "wifi.h"
#include "debug.h"
#include "alkomat.h"

using namespace WiFiManagement;


AsyncWebServer server(80);


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
