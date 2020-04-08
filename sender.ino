#include <Arduino.h>
#define dataPin 13
#define trigerPin 2
unsigned int printData[14] = {0x08, 0x61, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xFF};

void setup() {
  // put your setup code here, to run once:
  pinMode(trigerPin,INPUT_PULLUP);
  pinMode(dataPin, OUTPUT);
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(trigerPin)==0){
    for(int i =0; i <= 14 ;i++){
      if(i == 0){
        digitalWrite(dataPin,LOW);
        delay(50);
        digitalWrite(dataPin,HIGH);
        delay(1);
        }
      else{
      for(int j = 0; j <8 ; j++){
        //Serial.write(printData[i-1]);
        if(bitRead(printData[i-1],j)==1){
          digitalWrite(dataPin,LOW);
          delayMicroseconds(500);
          digitalWrite(dataPin,HIGH);
          delayMicroseconds(1000);
          digitalWrite(dataPin,LOW);
        }
        else{
          digitalWrite(dataPin,LOW);
          delayMicroseconds(1000);
          digitalWrite(dataPin,HIGH);
          delayMicroseconds(500);
          digitalWrite(dataPin,LOW);
        }
      }
      }
    }
  }
}
