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
#include <Servo.h>
#include "HX711.h"


/**----------------------------------------------
 * *       CONSTANTS AND DEFINITIONS
 *---------------------------------------------**/ 
#define SERVO_OPENPOS 180
#define SERVO_CLOSEPOS 0
#define STEPS_PER_ML 175 / 2
#define DELAYTIME_DURING_FILL 100


//todo  CHANGE THIS TO CLASS VARIABLE 
#define LOADCELL_DOUT_PIN D2
#define LOADCELL_SCK_PIN D3
 



#ifdef DEBUG
#define PRINT(s)            \
	{                       \
		Serial.print(F(s)); \
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
	Valve _valves[10];
	HX711 _scale;

	void initScale();

public:
	Alkomat();
	void addValve(uint8_t pin, drink_t drink);
	void changeDrink(Valve valve, drink_t newDrink);
	void pourDrink(uint8_t amount);
	void calibrateScale(float knownWeight);
};