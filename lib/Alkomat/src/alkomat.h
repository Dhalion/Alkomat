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



#define LOADCELL_DOUT_PIN D2
#define LOADCELL_SCK_PIN D3
#define ROLLING_AVG_N   5


/**============================================
**                Functions 
*=============================================**/

namespace Alkomat {
    void initScale();
    long readScale();
    long readAverageScale();

}


