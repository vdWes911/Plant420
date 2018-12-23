#include <Wire.h>

#define tempSensorAddress 0x40
#define SDA 18
#define SCL 19

#define RESET_CMD 0xFE
#define READ_TEMP_NOHOLD 0xF3
#define READ_RH_NOHOLD 0xF5

int samplesToAverage = 5;
unsigned long startTime;

void setup() {
  
  Serial.begin(9600);
  delay(50);
  
  StartSensor();
  Serial.println("Temperature,Humidity");
}

void loop() {
  
double averageTemperature = 0;
double averageHumidity = 0;

  for (int i = 0; i< samplesToAverage; i++)
  {
    double humidity = ReadHumidity();
    double temperature = ReadTemperature();
    
    if (temperature != NAN)
    {
      averageTemperature += temperature;
    }
    
    if (humidity != NAN)
    {
      averageHumidity += humidity;
    }

    delay(1000);
  }
  
  averageTemperature /= samplesToAverage;
  averageHumidity /= samplesToAverage;

  Serial.print(averageTemperature);
  Serial.print(",");
  Serial.println(averageHumidity);  
}

void StartSensor()
{
  Wire.begin(SDA, SCL);
  ResetTempSensor();
}

void ResetTempSensor()
{
  Wire.beginTransmission(tempSensorAddress);
  Wire.write(RESET_CMD);
  Wire.endTransmission();
  delay(50);
}

double ReadTemperature()
{
    int TRANSACTION_TIMEOUT = 100;
    
    Wire.beginTransmission(tempSensorAddress);
    Wire.write(READ_TEMP_NOHOLD);
    uint8_t err = Wire.endTransmission();

    if(err != 0)
    return NAN; //error
    
    startTime = millis();
    while(millis() - startTime < TRANSACTION_TIMEOUT)
    {
      if (Wire.requestFrom(tempSensorAddress, 3) == 3)
      {
        uint16_t temp = Wire.read() << 8 | Wire.read();
        uint8_t chxsum = Wire.read();
  
        double temperature = (double)temp;
        temperature *= 175.72;
        temperature /= 65536;
        temperature -= 46.85;
        return temperature;
      }
    delay(6); // 1/2 typical sample processing time
  }
  return NAN;
}

double ReadHumidity()
{
    int TRANSACTION_TIMEOUT = 100;
    
    Wire.beginTransmission(tempSensorAddress);
    Wire.write(READ_RH_NOHOLD);
    uint8_t err = Wire.endTransmission();

    if(err != 0)
    {
      return NAN; //error
    }
  
    startTime = millis();
    while(millis() - startTime < TRANSACTION_TIMEOUT)
    {
      if (Wire.requestFrom(tempSensorAddress, 3) == 3)
      {
        uint16_t hum = Wire.read() << 8 | Wire.read();
        uint8_t chxsum = Wire.read();
  
        float humidity = hum;
        humidity *= 125;
        humidity /= 65536;
        humidity -= 6;

        return humidity;
      }
    delay(6); // 1/2 typical sample processing time
  }
  return NAN;
}
