#include "alkomat.h"


using namespace Alkomat;

HX711 scale;


void Alkomat::initScale() {
    DebugPrintln("Initializing scale");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    while ((scale.is_ready()))
    {
        delay(200);
        DebugPrint(".")
    }
    scale.set_scale(97235);
    DebugPrintln("\nScale initialized.");
}

long Alkomat::readScale() {
    return scale.get_units();
}