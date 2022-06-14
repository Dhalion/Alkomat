#define DEBUG

#include <Arduino.h>
// #include "HX711.h"
// #include <Servo.h> 
// #include "Alkomat.h"

// HX711 circuit wiring 
#define LOADCELL_DOUT_PIN D2
#define LOADCELL_SCK_PIN D3
#define BUTTON_PIN D4




// WiFiManager
// Local intialization. Once its business is done, there is no need to keep it around

// TODO: FS WEBUI https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/

void setup()
{
  Serial.begin(115200);

}

void loop()
{

}