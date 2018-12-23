#include <Wire.h>

void scan(){
Serial.println(" Scanning I2C Addresses");
uint8_t cnt=0;
for(uint8_t i=0;i<128;i++){
  Wire.beginTransmission(i);
  uint8_t ec=Wire.endTransmission(true);
  if(ec==0){
    if(i<16)Serial.print('0');
    Serial.print(i,HEX);
    cnt++;
  }
  else Serial.print("..");
  Serial.print(' ');
  if ((i&0x0f)==0x0f)Serial.println();
  }
Serial.print("Scan Completed, ");
Serial.print(cnt);
Serial.println(" I2C Devices found.");
}

bool i2cReady(uint8_t adr){
uint32_t timeout=millis();
bool ready=false;
while((millis()-timeout<100)&&(!ready)){
  Wire.beginTransmission(adr);
  ready=(Wire.endTransmission()==0);
  }
return ready;
}

void setup(){
Serial.begin(115200);
Wire.begin(18,19, 400000);
scan();
Serial.println();
}

void loop(){}
