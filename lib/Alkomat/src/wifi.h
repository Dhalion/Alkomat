/**================================================================================================
 * ?                                           ABOUT
 * @author         :  Christopher Falkner
 * @email          :  christopher.falkner@web.de
 * @repo           :  -
 * @createdOn      :  14.06.22
 * @description    :  Alkomat (Longdrink Machine) WiFi Management
 *================================================================================================**/


#pragma once

#include <ESPAsync_WiFiManager.hpp>               //https://github.com/khoih-prog/ESPAsync_WiFiManager
 
#define AP_NAME "Alkomat-AP"
#define HOSTNAME "Alkomat"

namespace WiFiManagement 
{
    void initWifi();
}


