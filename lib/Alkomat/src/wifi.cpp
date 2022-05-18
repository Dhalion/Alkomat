#include "wifi.h"

// Cross reference Pointers
ESP8266WebServer*   server_p;
// Alkomat*            alkomat;




void WiFiHandler::initWifi()
{
    Serial.println("INIT WIFI");
    WiFi.setHostname(HOSTNAME);
    WiFiManager wifiManager;
    wifiManager.autoConnect("alkomat");
}

void WiFiHandler::startServer()
{
    Serial.println("Starting Server");
    _server = new ESP8266WebServer(80);
    // Setting cross reference pointers
    server_p =_server;
    // alkomat = _alkomat;
    _server->on("/test", endpointTest);
    _server->on("/status", endpointStatus);
    _server->on("/pour", endpointPour);


    _server->onNotFound(handleNotFound);
    _server->begin();
}

// void WiFiHandler::handle()
// {
//     _server->handleClient();
// }



void endpointTest()
{
    server_p->send(200,"text/plain", "200: OK");
    Serial.println("+++ ENDPOINT TEST +++");
}

void handleNotFound()
{
    server_p->send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void endpointStatus() {
    server_p->send(200, "application/json", "{\"status\": \"healthy\"}"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void endpointPour() {
    /**----------------------
     *    URL Parameters:
     *      - amount    (mandatory)
     *      - valve     (either this or drink)
     *      - drink     (either this or valve)
     *------------------------**/

    //*     Variables       *//
    String amount;
    String valve;
    String drink;
    StaticJsonDocument<200> json; 
    String response;


    // Check if params are not empty
    if (server_p->args() < 1) {
        return;
    }
    // Check if amount is given
    if (!server_p->hasArg("amount")) {
        return;
    }
    amount = server_p->arg("amount");

    // Check if valve or drink is given
    if (! (server_p->hasArg("drink") || server_p->hasArg("valve"))) {
        return;
    }

    // Reading parameters
    valve = server_p->arg("valve");
    drink = server_p->arg("drink");

    // Assemble response JSON
    json["drink"]   = drink;
    json["valve"]   = valve;
    json["amount"]  = amount;
    
    serializeJson(json, response);

    // pourDrink(amount.toInt());

    server_p->send(200, "application/json", response); 
}
