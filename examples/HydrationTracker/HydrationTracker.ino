#include "HX711.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define calibration_factor -218

BluetoothSerial SerialBT;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 23;
const int LOADCELL_SCK_PIN = 22;

HX711 scale;
long previous_reading = 0;

float batVoltage = 0;

void setup() 
{
    Serial.begin(115200);
    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(calibration_factor);
    scale.tare();
    pinMode(A13, INPUT);
}

void loop() 
{

    if (scale.is_ready())
    {
        long reading = scale.get_units(100);
        
        if (reading > 100)
        {
          Serial.println("Reading Set...");
          delay(2000);
        
          if (reading > previous_reading + 12) 
          {
            previous_reading = reading;
            Serial.print("HX711 reading (g): ");
            Serial.println(reading);  
          }
          else if (reading < previous_reading - 12)
          {
            long delta = previous_reading - reading;
            previous_reading = reading;
            Serial.println("HX711 delta (g): ");
            //BT_print();
            Serial.println(delta);
            //BT_print();
          }
        }
    } 
    else 
    {
        Serial.println("HX711 not found.");
    }

    //batVoltage = analogRead(A13);
    //batVoltage = (batVoltage*(3.7/4095.0))*2.0;
    //Serial.println("Battery Voltage (V):");
    //Serial.println(batVoltage);

    Serial.println("Not Reading");
    BT_print(100);
    delay(3000);

}

void BT_print(int val)
{
  char arr[100];
  sprintf(arr, "%d\r\n", val);
  
  SerialBT.write((uint8_t*)arr, sizeof(val)+1);
    
}
