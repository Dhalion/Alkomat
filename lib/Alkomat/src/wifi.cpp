#include <ESPAsync_WiFiManager.h>               //https://github.com/khoih-prog/ESPAsync_WiFiManager


#include "wifi.h"

using namespace WiFiManagement;


void WiFiManagement::initWifi(){
    AsyncWebServer  _ws(80);
    DNSServer       _dnss;
    ESPAsync_WiFiManager wifiManager(&_ws, &_dnss);
    DebugPrintln("Starting WiFi autoconnect...");
    wifiManager.autoConnect(AP_NAME);
    WiFi.hostname(HOSTNAME);
    DebugPrintln("Wifi connected.");
}
