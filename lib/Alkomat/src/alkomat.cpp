#include "alkomat.h"
#include "debug.h"


using namespace Alkomat;

HX711 scale;
movingAvg averageReading(ROLLING_AVG_N);


void Alkomat::initScale() {
    DebugPrintln("Initializing scale");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    while ((scale.is_ready()))
    {
        delay(200);
        DebugPrint(".");
    }
    // scale.set_scale(97235);
    averageReading.begin();
    DebugPrintln("\nScale initialized.");
}

long Alkomat::readScale() {
    return -scale.get_units();  // Scale mounted upside down so *-1
}


long Alkomat::readAverageScale() {
    return averageReading.reading(readScale());
}
