/**================================================================================================
 * ?                                           ABOUT
 * @author         :  Christopher Falkner
 * @email          :  christopher.falkner@web.de
 * @repo           :  -
 * @createdOn      :  14.06.22
 * @description    :  Alkomat (Longdrink Machine) Management and Control Class (Remake)
 *================================================================================================**/

#pragma once


/**============================================
 **               INCLUDES
 *=============================================**/
#include <Arduino.h>
#include "HX711.h"
#include <movingAvg.h>                  // https://github.com/JChristensen/movingAvg

#include "debug.h"
#include "cppQueue.h"



#define LOADCELL_DOUT_PIN D2
#define LOADCELL_SCK_PIN D3
#define ROLLING_AVG_N   5
#define SCALE_CALIBRATION_FACTOR -973

// Command codes to place in queue
enum CommandType {
    calibrate
};


enum drinkType_t {
    water,
    cola,
    wodka
};

struct Command_t {
    CommandType type;
    int int_parm;
};

struct Valve_t {
    int id;
    bool state;
    drinkType_t drinkType;
};





/**============================================
**                Functions 
*=============================================**/

namespace Alkomat {
    void initScale();
    void handle();
    void addCommandToQueue(Command_t command);
    long readScale();
    long readAverageScale();
    void calibrateScale(int knownWeight);
    void fill(int amount);
    void setValve(Valve_t v, bool state);
    void testValveFunctions();
}


