// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not known.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>


#define COMMAND_BIT  0x80
#define CONTROL_REG  0x00
#define DATA_LOW_0   0x0C
#define DATA_HIGH_0  0x0D
#define DATA_LOW_1   0x0E
#define DATA_HIGH_1  0x0F

#define POWER_ON     0x03
#define POWER_OFF    0x00
#define WORD_READ    0x10

byte address1 = 0;
void setup()
{
  Wire.begin(18,19);
  Wire.setClock(400000);
  pinMode(17, OUTPUT);
  
  digitalWrite(17, HIGH);
  delay(100);
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}


void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission(true);
    
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      if (address != 0x40)
      {
        address1 = address;
        PowerOn(address1);
        ReadID(address1);
        
        //double value = ReadLight(address1);
        //Serial.println("Read Value:");
        //Serial.println(value);
      }
      
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}

bool PowerOn(int address)
{
  Wire.flush();
  Wire.beginTransmission(address);
  Wire.write(0x80 | 0x00);
  delay(20);
  Wire.write(0x03);
  delay(20);
  
  uint8_t err = Wire.endTransmission();
  delay(10);
  if(err != 0)
  {
    Serial.print("Could not turn on the sensor. Error occured: ");
    Serial.println(err);
    return false;
  }
  {
    Serial.println("Powered On");
    return true;
  }
}

uint8_t ReadID(int add)
{
    Wire.beginTransmission(add);
    Wire.write(COMMAND_BIT | 0x0A);
    uint8_t err = Wire.endTransmission();

    delay(100);
    if(Wire.requestFrom(add, 2) == 2)
    {
      uint8_t value = Wire.read() << 8 |Wire.read();
      Serial.println("******");
      Serial.println("ID");
      Serial.println(value);
      return value;
    }
    else
  {
    Serial.println("BROKEH");
    return -1;
  }
}


double ReadLight(int add)
{
    int TRANSACTION_TIMEOUT = 100;
    uint16_t mixedLightRaw = 0;
    uint16_t irLight = 0;
    
    Wire.beginTransmission(add);
    Wire.write(COMMAND_BIT | WORD_READ);
    Wire.write(DATA_LOW_0);
    uint8_t err = Wire.endTransmission();

    if(err != 0)
    return NAN; //error
    
    unsigned long startTime = millis();
    while(millis() - startTime < TRANSACTION_TIMEOUT)
    {
      if (Wire.requestFrom(add, 2) == 2)
      {
        mixedLightRaw = Wire.read() << 8 | Wire.read();
      }
      delay(6); // 1/2 typical sample processing time
    }
    

    Wire.beginTransmission(add);
    Wire.write(COMMAND_BIT | WORD_READ);
    Wire.write(DATA_LOW_1);
    err = Wire.endTransmission();

    if(err != 0)
    Serial.println("Error in mixed Light.");
    return NAN; //error
    
    startTime = millis();
    while(millis() - startTime < TRANSACTION_TIMEOUT)
    {
      if (Wire.requestFrom(add, 2) == 2)
      {
        irLight = Wire.read() << 8 | Wire.read();
      }
      delay(6); // 1/2 typical sample processing time
    }

    if (irLight == 0)
    {
      Serial.println("IR light is empty? WTF?");
    }

    if (mixedLightRaw == 0)
    {
      Serial.println("mixed light is empty? WTF?");
    }
    else
    {
      return (double)mixedLightRaw;
    }

    return NAN;
}
