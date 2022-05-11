/**================================================================================================
 * ?                                           ABOUT
 * @author         :  Christopher Falkner
 * @email          :  christopher.falkner@web.de
 * @repo           :  -
 * @createdOn      :  10.05.22
 * @description    :  Alkomat (Longdrink Machine) Management and Control Class
 *================================================================================================**/


#include "Alkomat.h"


/**--------------------------------------------
 *               VALVE CLASS
 *---------------------------------------------**/
uint8_t Valve::setPin(uint8_t newPin)
{
    if (newPin)
    {
        this->_pin = newPin;
        PRINT("\nError check"); 
        Serial.println("Hello?");
        return this->_pin;
    }
    else
    {
        PRINT("[VENTIL][Valve ] - Error while setting new Pin.")
        return -1;
    }
}

void Valve::open(){
    this->_servoObject.write(SERVO_OPENPOS);
    this->_state = valve_state_t::open;
}


void Valve::close(){
    this->_servoObject.write(SERVO_CLOSEPOS);
    this->_state = valve_state_t::closed;
}


bool Valve::status(){
    return (this->_state);
}

uint8_t Valve::init(uint8_t pin, drink_t drink){
    PRINT("\n INITIALIZE VALVE");
    Serial.println("Valve INITIALIZE VALVE");
    if (this->_pin) {
        this->_servoObject.attach(this->_pin);
        this->close();

        return 0;
    }
    else return -1;
}


uint8_t Valve::setDrink(drink_t drink) {
    if (drink) {
        this->_drink = drink;
        return this->_drink;
    }
    else return -1;
}



/**--------------------------------------------
 *               ALKOMAT CLASS
 *---------------------------------------------**/

Alkomat::Alkomat(){

}


void Alkomat::initScale(){
    this->_scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void Alkomat::addValve(uint8_t pin, drink_t drink){
    _valves[0] = Valve();
    _valves[0].init(pin, drink);
    _valves[0].close();
}

void Alkomat::changeDrink(Valve valve, drink_t drink){

}

void Alkomat::calibrateScale(float knownWeight){
    PRINT("[!] Calibration START");
    Serial.println("Calibration START");
    long reading = 0;
    while (!this->_scale.is_ready()) {
        delay(500);
        PRINT("Waiting");
    }
    reading = this->_scale.get_units();
    PRINT("Place known Weight on Scale...\n");
    // Wait until something is placed on scale
	while (this->_scale.get_units() - reading < 10)     
	{
		delay(100);
	}
	reading = this->_scale.get_units(10);
	PRINT("GET UNITS:\t" + reading + "\n");
	PRINT("Assuming Mass of 154g; This results in factor " +  reading / 154 + "\n");
	PRINT("Calibrating with recorded values...\n");

	PRINT("Remove Object from scale and press \"C\"\n");
	delay(1000);


	this->_scale.set_scale(reading / 154);
	this->_scale.tare();

}

void Alkomat::pourDrink(uint8_t amount){
    PRINT("[#] Pouring {amount} ml...\n");

    //! ONLY FOR DEBUGGING
    Valve valve = _valves[0];

    long _startWeight   = this->_scale.read_average(10);        // Get the start weight
    long _endWeight     = _startWeight + amount;                // Calculate the end weight
    amount = amount * STEPS_PER_ML;
    valve.open();                                               // Open Valve
    // wait until amount is filled
    while (this->_scale.read_average(10) < _endWeight) {
        // wait
        delay(DELAYTIME_DURING_FILL);
    }
    valve.close();
    PRINT("[#] Closing Valve.\n");
}

