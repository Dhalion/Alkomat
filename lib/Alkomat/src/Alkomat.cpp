/**================================================================================================
 * ?                                           ABOUT
 * @author         :  Christopher Falkner
 * @email          :  christopher.falkner@web.de
 * @repo           :  -
 * @createdOn      :  10.05.22
 * @description    :  Alkomat (Longdrink Machine) Management and Control Class
 *================================================================================================**/

#include "Alkomat.h"

AsyncWebServer  *server_p;
Alkomat *alkomat_p;

/**--------------------------------------------
 *               VALVE CLASS
 *---------------------------------------------**/
Valve::Valve() {}

Valve::Valve(uint8_t pin)
{
    this->_servoObject.attach(pin);
    this->_pin = pin;
    this->close();
}

uint8_t Valve::setPin(uint8_t newPin)
{

    if (newPin)
    {
        this->_pin = newPin;
        PRINT("[Valve] - New Pin set.")
        return this->_pin;
    }
    else
    {
        PRINT("[Valve] - Error while setting new Pin.")
        return -1;
    }
}

void Valve::open()
{
    this->_servoObject.write(SERVO_OPENPOS);
    this->_state = valve_state_t::open;
}

void Valve::close()
{
    this->_servoObject.write(SERVO_CLOSEPOS);
    this->_state = valve_state_t::closed;
}

bool Valve::status()
{
    return (this->_state);
}

uint8_t Valve::init(uint8_t pin, drink_t drink)
{
    PRINT("\n INITIALIZE VALVE\n");
    if (this->_pin)
    {
        this->_servoObject.attach(this->_pin);
        this->close();

        return 0;
    }
    else
        return -1;
}

uint8_t Valve::setDrink(drink_t drink)
{
    if (drink)
    {
        this->_drink = drink;
        return this->_drink;
    }
    else
        return -1;
}

/**--------------------------------------------
 *               ALKOMAT CLASS
 *---------------------------------------------**/

Alkomat::Alkomat()
{
    alkomat_p = this;
    initScale();
}

void Alkomat::initScale()
{
    PRINT("[#] Init Scale...\n");
    this->_scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    while ((!this->_scale.is_ready()))
    {
        delay(100);
    }
    _scale.set_scale(97235);
    PRINT("[#] Scale initialized.\n");
}

void Alkomat::addValve(uint8_t pin, drink_t drink)
{
    _valves[0] = Valve(pin);
    _valves[0].close();
}

void Alkomat::changeDrink(Valve valve, drink_t drink)
{
}

void Alkomat::calibrateScale(float knownWeight)
{
    /**=======================
     * todo      ADD CALIBRATION VIA WEB INTERFACE
     *
     *
     *========================**/
    // PRINT("[!] Calibration START");
    long reading = 0;

    Serial.println("Remove all weight from scale");

    _scale.set_scale();
    _scale.tare(); // Reset the scale to 0

    long zero_factor = -_scale.read_average();  // Get a baseline reading
    Serial.print("Zero factor: ");              // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);

    Serial.println("Calibration START");
    while (!this->_scale.is_ready())
    {
        delay(500);
        PRINT("Waiting\n");
    }
    reading = -this->_scale.get_units(5);
    PRINT("Place known Weight on Scale...\n");
    Serial.printf_P("Used Weight: %f\n", knownWeight);
    // Wait until something is placed on scale
    delay(500);
    for (int i = 5; i > 0; i--)
    {
        Serial.printf_P("Continuing in %ds...\r", i);
        delay(1000);
    }

    reading = -this->_scale.get_units(10);
    PRINT(("GET UNITS:\t" + reading));
    PRINT("\nThis results in factor " + (int)(reading / knownWeight));
    PRINT("\nCalibrating with recorded values...\n");

    PRINT("Remove Object from scale!\n");
    for (int i = 5; i > 0; i--)
    {
        Serial.printf_P("Continuing in %ds...\r", i);
        delay(1000);
    }
    delay(1000);

    this->_scale.set_scale(reading / knownWeight);
    this->_scale.tare();
    PRINT("[#] Calibration Complete.\n");
}

void Alkomat::pourDrink(uint8_t amount)
{
    long _currentWeight = 0;
    Serial.printf_P("Pouring %dml\n", amount);

    //! ONLY FOR DEBUGGING:
    Valve valve = _valves[0];

    long _startWeight = -this->_scale.get_units(5); // Get the start weight
    long _endWeight = _startWeight + amount;            // Calculate the end weight
    amount = amount * STEPS_PER_ML;

    Serial.printf_P("Start Weight is %ld\n", _startWeight);
    Serial.printf_P("End Weight is %ld\n", _endWeight);

    _currentWeight = _startWeight;

    valve.open(); // Open Valve
    // wait until amount is filled
    while (_currentWeight < _endWeight)
    {
        // wait
        delay(DELAYTIME_DURING_FILL);
        Serial.printf_P("Left to fill: %ld\n", _endWeight - _currentWeight);
        _currentWeight = -this->_scale.get_units(5);
    }
    delay(100);
    valve.close();
    PRINT("[#] Closing Valve.\n");
}

void Alkomat::getStatus()
{
    PRINT("[#] Get Status");
    if (this->_scale.is_ready())
    {
        PRINT("[+] Scale Ready.");
    }
}

void Alkomat::pourAmount(uint8_t amount, Valve valve)
{
    long _startWeight = this->_scale.get_units(5);
    long _endWeight = _startWeight + amount;
    amount = amount * STEPS_PER_ML;

    valve.open(); // Open Valve
    // wait until amount is filled
    while (this->_scale.get_units(5) < _endWeight)
    {
        // wait
        delay(DELAYTIME_DURING_FILL);
    }
    valve.close();
}

void Alkomat::startServer()
{
    Serial.println("Starting Server");
    _server = new AsyncWebServer (80);
    // Setting cross reference pointers
    server_p = _server;
    _server->on("/test", HTTP_GET, [](AsyncWebServerRequest *request){ endpointTest(request); });
    _server->on("/status", HTTP_GET, [](AsyncWebServerRequest *request){ endpointStatus(request); });
    _server->on("/pour", HTTP_GET, [](AsyncWebServerRequest *request){ endpointPour(request); });
    _server->on("/valve", HTTP_GET, [](AsyncWebServerRequest *request){ endpointValve(request); });
    _server->on("/calibrate", HTTP_GET, [](AsyncWebServerRequest *request){ endpointValve(request); });
    _server->on("/scale", HTTP_GET, [](AsyncWebServerRequest *request){ endpointScale(request); });

    _server->onNotFound(handleNotFound);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    // _server->enableCORS(true);
    _server->begin();
}

void Alkomat::initWifi()
{
    Serial.println("INIT WIFI");
    WiFi.setHostname(HOSTNAME);
    AsyncWebServer server(80);
    DNSServer dns;
    AsyncWiFiManager wifiManager(&server,&dns);
    wifiManager.autoConnect("alkomat");
}

void Alkomat::handleWiFi()
{
    // _server->handleClient();
}

void Alkomat::handleCallbacks(){

    // Check for task
    if (_task.done){} {
        Serial.println(_task.done);
        return;
    }
    PRINT("Task found\n");

    // Execute
    if (_task.action == action_t::pour) {
        PRINT("Action is [pour]");
        pourDrink(_task.amount);
    }

    PRINT("Task executed. Delete now...\n");
    // Delete executed task
    _task.done = true;

    return;
}

/**------------------------------------------------------------------------
 *                           URL ENDPOINTS
 *------------------------------------------------------------------------**/

void endpointTest(AsyncWebServerRequest *request)
{
    request->send(200, "text/plain", "200: OK");
    Serial.println("+++ ENDPOINT TEST +++");
}

void handleNotFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void endpointStatus(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", "{\"status\": \"healthy\"}"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void endpointPour(AsyncWebServerRequest *request)
{
    /**----------------------
     *    URL Parameters:
     *      - amount    (mandatory)
     *      - valve     (either this or drink)
     *      - drink     (either this or valve)
     *------------------------**/

    //*     Variables       *//
    int amount;
    String valve;
    String drink;
    StaticJsonDocument<200> json;
    String response;

    // Check if params are not empty
    if (request->args() < 1)
    {
        return;
    }
    // Check if amount is given
    if (!request->hasArg("amount"))
    {
        return;
    }
    amount = request->arg("amount").toInt();

    // Check if valve or drink is given
    if (!(request->hasArg("drink") || request->hasArg("valve")))
    {
        return;
    }

    // Reading parameters
    valve = request->arg("valve");
    drink = request->arg("drink");

    // Assemble response JSON
    json["drink"] = drink;
    json["valve"] = valve;
    json["amount"] = amount;

    serializeJson(json, response);



    // alkomat_p->pourDrink(amount.toInt());
    // Add Task instead of this
    PRINT("Task added");
    alkomat_p->_task.done = false;
    alkomat_p->_task.amount = amount;
    alkomat_p->_task.action = action_t::pour;

    request->send(200, "application/json", response);
    PRINT("Return");
    return;

}

void endpointValve(AsyncWebServerRequest *request)
{
    int valve;
    String action;
    StaticJsonDocument<200> json;
    String response;

    // Check if params are not empty
    if (request->args() < 1)
    {
        return;
    }
    // Check if amount is given
    if (!request->hasArg("valve"))
    {
        return;
    }
    valve = request->arg("valve").toInt();

    if (!request->hasArg("valve"))
    {
        return;
    }
    action = request->arg("action");

    if (action == "open")
    {
        alkomat_p->_valves[valve].open();
    }
    else if (action == "close")
    {
        alkomat_p->_valves[valve].close();
    }

    json["valve"] = valve;
    json["action"] = action;

    serializeJson(json, response);

    request->send(200, "application/json", response);
}

void endpointCalibrateScale(AsyncWebServerRequest *request)
{
    int _knownWeight = 0;

    // Check if params are not empty
    if (request->args() < 1)
    {
        return;
    }
    // Check if amount is given
    if (!request->hasArg("weight"))
    {
        return;
    }
    _knownWeight = request->arg("weight").toFloat();

    alkomat_p->calibrateScale(_knownWeight);

    request->send(200);
}

void endpointScale(AsyncWebServerRequest *request)
{
    long _weight = 0;
    StaticJsonDocument<200> json;
    String response;

    if (!alkomat_p->_scale.is_ready())
    {
        request->send(503);
        return;
    }
    _weight = -alkomat_p->_scale.get_units(5);

    json["weight"] = _weight;

    serializeJson(json, response);

    request->send(200, "application/json", response);
}