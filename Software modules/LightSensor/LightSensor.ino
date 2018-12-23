#include <Wire.h>

// Sensor control defines

#define COMMAND_BIT  0x80
#define CONTROL_REG  0x00
#define DATA_LOW_0   0x0C
#define DATA_HIGH_0  0x0D
#define DATA_LOW_1   0x0E
#define DATA_HIGH_1  0x0F

#define POWER_ON     0x03
#define POWER_OFF    0x00
#define WORD_READ    0x10

#define LightAddress 0x23 // Try to send the power on command to all addresses except 0x40 (The temp sensor)
#define SDA 18
#define SCL 19

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(17, OUTPUT);

  digitalWrite(17, HIGH);
  delay(100);

  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  initLightSensor();
}

void loop() {
  double fakeLuxValue = ReadLight();
  Serial.print("Light sensor value: ");
  Serial.println(fakeLuxValue);
  delay(1000);
}

void initLightSensor()
{
  Wire.begin(SDA,SCL);
  PowerOn();
}

bool PowerOn()
{
  Wire.beginTransmission(LightAddress);
  Wire.write(COMMAND_BIT | CONTROL_REG);
  delay(20);
  Wire.write(POWER_ON);
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
    return true;
  }
}

bool PowerOff()
{
  Wire.beginTransmission(LightAddress);
  Wire.write(COMMAND_BIT | CONTROL_REG);
  Wire.write(POWER_OFF);
  uint8_t err = Wire.endTransmission();
  if(err != 0)
  {
    Serial.print("Could not turn off the sensor. Error occured: ");
    Serial.println(err);
    return false;
  }
  {
    return true;
  }
}

double ReadLight()
{
    int TRANSACTION_TIMEOUT = 100;
    uint16_t mixedLightRaw = 0;
    uint16_t irLight = 0;
    
    Wire.beginTransmission(LightAddress);
    Wire.write(COMMAND_BIT | WORD_READ);
    Wire.write(DATA_LOW_0);
    uint8_t err = Wire.endTransmission();

    if(err != 0)
    return NAN; //error
    
    unsigned long startTime = millis();
    while(millis() - startTime < TRANSACTION_TIMEOUT)
    {
      if (Wire.requestFrom(LightAddress, 2) == 2)
      {
        mixedLightRaw = Wire.read() << 8 | Wire.read();
      }
      delay(6); // 1/2 typical sample processing time
    }
    

    Wire.beginTransmission(LightAddress);
    Wire.write(COMMAND_BIT | WORD_READ);
    Wire.write(DATA_LOW_1);
    err = Wire.endTransmission();

    if(err != 0)
    return NAN; //error
    
    startTime = millis();
    while(millis() - startTime < TRANSACTION_TIMEOUT)
    {
      if (Wire.requestFrom(LightAddress, 2) == 2)
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
