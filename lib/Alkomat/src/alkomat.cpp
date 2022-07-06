#include "alkomat.h"
#include "debug.h"
#include "wifi.h"

using namespace Alkomat;

HX711 scale;
movingAvg averageReading(ROLLING_AVG_N);
cppQueue commandQueue(sizeof(Command), 10, FIFO); // Instanciate the command queue

void Alkomat::initScale()
{
    DebugPrintln("Initializing scale");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    while ((scale.is_ready()))
    {
        delay(200);
        DebugPrint(".");
    }
    scale.set_scale(SCALE_CALIBRATION_FACTOR);
    averageReading.begin();
    DebugPrintln("\nScale initialized.");
}

long Alkomat::readScale()
{
    return scale.get_units() - 100; // -100g Correction
}

long Alkomat::readAverageScale()
{
    return averageReading.reading(readScale());
}

void Alkomat::handle()
{
    Command cmd;
    if (!commandQueue.isEmpty())
    {
        // Execute command(s)

        for (int i = 0; i < commandQueue.getCount(); i++)
        {
            if (commandQueue.pop(&cmd))
            { // Read and Remove first element
                DebugPrintln("Read and Removed successfully!");
            }
            else
            {
                DebugPrintln("Read and Remove failed!");
            }

            // What is the command?
            switch (cmd.type)
            {
            case CommandType::calibrate:
                DebugPrintln("# Executing Calibration command...");
                Alkomat::calibrateScale(cmd.int_parm);
                DebugPrintln("# Finished Calibration command...");

                break;

            default:
                DebugPrintln("# Command not known!");
                break;
            }
        }

        // Queue should be empty now
        if (commandQueue.getCount() > 0)
        {
            // Force flush if not already empty
            DebugPrintln("Queue not empty! Clearing...");
            commandQueue.flush();
        }
    }
}

void Alkomat::addCommandToQueue(Command command)
{
    if (!commandQueue.isFull())
    {
        commandQueue.push(&command);
    }
    else
    {
        DebugPrintln("Error while Pushing to Queue");
    }
}

void Alkomat::calibrateScale(int knownWeight)
{
    long result, start;
    int timer;

    WiFiManagement::addToDebugLog("=== Calibration Start ===");
    Serial.println("=== Calibration Start ===");
    WiFiManagement::addToDebugLog("# Remove any Weights on the scale!");
    Serial.println("# Remove any Weights on the scale!");

    DebugPrintln("Taring...");
    scale.tare();

    WiFiManagement::addToDebugLog("# Tare done.");
    Serial.println("Tare done.");

    WiFiManagement::addToDebugLog(("# Place the known Weight of " + String(knownWeight) + "g on the scale."));
    Serial.println(("# Place the known Weight of " + String(knownWeight) + "g on the scale."));

    delay(7000);

    DebugPrintln("Reading Weight...");
    result = readAverageScale();
    WiFiManagement::addToDebugLog("# Result: " + String(result) + "\n# Calibration Factor: " + String(result / knownWeight));
    Serial.println("\n# Result: " + String(result) + "\n# Calibration Factor: " + String(result / knownWeight));


    DebugPrintln("Set scale...");
    scale.set_scale(result / knownWeight);

    WiFiManagement::addToDebugLog("=== Calibration Done ===");
    Serial.println("=== Calibration Done ===");
}
