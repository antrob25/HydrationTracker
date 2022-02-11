#include "HX711.h"

#define calibration_factor -7050.0

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

float batVoltage = 0;

void setup() 
{
    Serial.begin(9600);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(calibration_factor);
    pinMode(A13, INPUT);
}

void loop() 
{
    if (scale.is_ready())
    {
        long reading = scale.read();
        Serial.print("HX711 reading: ");
        Serial.println(reading);
    } 
    else 
    {
        Serial.println("HX711 not found.");
    }

    batVoltage = analogRead(A13);
    batVoltage = (batVoltage*(3.7/4095.0))*2.0;
    Serial.println("Battery Voltage (V):");
    Serial.println(batVoltage);

    delay(1000);

}