#include "HX711.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define calibration_factor -7050.0

BluetoothSerial SerialBT;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

float batVoltage = 0;

void setup() 
{
    Serial.begin(115200);
    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(calibration_factor);
    pinMode(A13, INPUT);
}

void loop() 
{
    if (Serial.available()) 
    {
        while (Serial.available())
        {
            SerialBT.write(Serial.read());
        }    
    }
    if (SerialBT.available()) 
    {
        while (SerialBT.available())
        {
            Serial.write(SerialBT.read());

        }
    }
    delay(100);

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
