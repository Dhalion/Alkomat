#define DEBUG

#include <Arduino.h>
#include "HX711.h"
#include "Alkomat.h"
// HX711 circuit wiring

#define LOADCELL_DOUT_PIN D2
#define LOADCELL_SCK_PIN D3
#define BUTTON_PIN D4

#define AMOUNT_TO_FILL 200
#define STEPS_PER_ML 175 / 2


Alkomat alkomat;

void setup()
{
  Serial.begin(115200);
  PRINT("Calibrating...");
  alkomat.calibrateScale(154);
  PRINT("Calibrated.");

  alkomat.addValve(D5, drink1);

}

void loop()
{
  alkomat.pourDrink(50);
  delay(5000);
}