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
          BT_print("Reading Set...");
          delay(2000);
        
          if (reading > previous_reading + 12) 
          {
            previous_reading = reading;
            BT_print("HX711 reading (g): ");
            BT_print((uint16_t)reading);  
          }
          else if (reading < previous_reading - 12)
          {
            long delta = previous_reading - reading;
            previous_reading = reading;
            BT_print("HX711 delta (g): ");
            BT_print((uint16_t)delta);
          }
        }
    } 
    else 
    {
        BT_print("HX711 not found.");
        BT_printNewline();
    }

    batVoltage = analogRead(A13);
    batVoltage = (batVoltage*(3.7/4095.0))*2.0;
    BT_print("Battery Voltage (V):  ");
    BT_print((uint8_t)batVoltage);
    BT_printNewline();
    BT_print("Not Reading ");
    BT_printNewline();
    delay(1500);

}

void BT_print(uint8_t val)
{
  char arr[100];
  sprintf(arr, "%d", val);
  SerialBT.write((uint8_t)arr[0]);
  if ( val >= 10 )
  {
    SerialBT.write((uint8_t)arr[1]);
  }
  if ( val > 100)
  {
    SerialBT.write((uint8_t)arr[2]);
  }
}

void BT_print(uint16_t val)
{
  char arr[100];
  sprintf(arr, "%d", val);
  SerialBT.write((uint8_t)arr[0]);
  if ( val >= 10 )
  {
    SerialBT.write((uint8_t)arr[1]);
  }
  if ( val > 100)
  {
    SerialBT.write((uint8_t)arr[2]);
  }
  if ( val > 255 )
  {
    SerialBT.write((uint8_t)arr[3]);
  }  
  if ( val >= 1000 )
  {
    SerialBT.write((uint8_t)arr[4]);
  }
  if ( val > 10000)
  {
    SerialBT.write((uint8_t)arr[5]);
  }
}

void BT_print(String s)
{
  uint8_t len = s.length();
  unsigned char word[len];
  s.getBytes(word, len);
  SerialBT.write((uint8_t*)word, len);
}

void BT_printNewline()
{
  char arr[2] = {'\r', '\n'};
  SerialBT.write((uint8_t*)arr, sizeof(arr));
}