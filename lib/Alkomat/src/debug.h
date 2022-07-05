#pragma once

#define DEBUG

#ifdef DEBUG 
  #define DebugPrint(...)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.print(__LINE__);     \
        Serial.print(' ');      \
        Serial.print(__VA_ARGS__)
  #define DebugPrintln(...)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.print(__LINE__);     \
        Serial.print(' ');      \
        Serial.println(__VA_ARGS__)
    #define DebugTest Serial.println("DEBUG IS ENABLED");
#else
  #define DebugPrint(...)
  #define DebugPrintln(...)  
#endif