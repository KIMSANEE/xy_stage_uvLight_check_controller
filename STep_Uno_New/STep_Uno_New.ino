#include <ArduinoJson.h>
#include <Wire.h>
#include "Adafruit_SI1145.h"

int PULx=3; //define Pulse pin
int DIRx=4; //define Direction pin
int ENAx=2; //define Enable Pin
int PULy=9; //define Pulse pin
int DIRy=10; //define Direction pin
int ENAy=8; //define Enable Pin
int SwX = 5;
int SwY = 11;
int flagX = 0;
int flagY = 0;
int SwStateX = 0; 
int SwStateY = 0;
StaticJsonDocument<100> doc;
Adafruit_SI1145 uv = Adafruit_SI1145();
JsonArray value = doc.createNestedArray("value");
float UVindex = 0.0;
float sum_UVindex = 0.0;
int distance, num, standard, check;
float valocity;
String message;
// default Center
int Center_X = 136;
int Center_Y = 148;

void setup() {
  pinMode (PULx, OUTPUT);
  pinMode (DIRx, OUTPUT);
  pinMode (ENAx, OUTPUT);
  pinMode (SwX, INPUT_PULLUP);
  pinMode (PULy, OUTPUT);
  pinMode (DIRy, OUTPUT);
  pinMode (ENAy, OUTPUT);
  pinMode (SwY, INPUT_PULLUP);
  Serial.begin(115200);
  delay(100);
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }
  while (!Serial) continue;
  
}

void loop(){

  if(Serial.available()){
    if(check==1){
      message = Serial.readStringUntil('\n');
      if(message=="Initialize"){
        flagX = 0;
        flagY = 0;
        delay(5);
        Center_X = Serial.parseInt();
        Center_Y = Serial.parseInt();
        while(flagY == 0){
          moveToZero(Center_X,Center_Y);
        }
      } else if (message == "Go"){
        moveLeftX(20*standard,150);
        moveLeftY(20*standard,150);
      } else if (message == "DataRead"){
        DataRead(num);
      } else if (message == "moveLeftX"){
        moveLeftX_silent(40*distance,125/valocity);
      } else if (message == "moveRightX"){
        moveRightX_silent(40*distance,125/valocity);
      } else if (message == "moveRightY"){
        moveRightY_silent(40*distance,125/valocity);
      } else if (message == "moveRightY"){
        moveLeftY_silent(40*distance,125/valocity);
      } else if (message == "done"){
        check = 0;
      } else {
        delay(10);
      }
      
    } 
     else {
      num = Serial.parseInt();
      standard = Serial.parseInt();
      valocity = Serial.parseFloat();
      distance = Serial.parseInt();
      check = Serial.parseInt();
      // num = 1000;
      // standard = 100;
      // valocity = 1;
      // distance = 2;
      // check = 1;
    }
  }

    
}

void DataRead(int num){
  for(int i=0; i<num; i++){
    UVindex = uv.readVisible();
    sum_UVindex += UVindex;
    delayMicroseconds(150);
  }
  Serial.println(sum_UVindex/num);
  sum_UVindex = 0;
}

void moveToZero(int _Center_X, int _Center_Y){
    SwStateX = digitalRead(SwX);
    SwStateY = digitalRead(SwY);
    if(SwStateX==LOW){
      if(flagX == 1){
        if(SwStateY==LOW){
          if(flagY == 1){
            digitalWrite(ENAx, HIGH);
            digitalWrite(ENAy, HIGH);
          } else{
            Ini_moveLeftY(100);
          }
        } else {
            delay(1000);
            moveRightY(40*_Center_Y,100);
            flagY = 1;
        }
      } else{
        Ini_moveLeftX(100);
      }
    } else {
      delay(1000);
      moveRightX(40*_Center_X,100);
      flagX = 1;
    }
}

void Ini_moveLeftX(int speed) {
  digitalWrite(DIRx, HIGH);
  digitalWrite(ENAx, LOW);
  digitalWrite(PULx, HIGH);
  delayMicroseconds(speed);
  digitalWrite(PULx, LOW);
  delayMicroseconds(speed);
}

void moveLeftX(int pulse, int speed) {
  digitalWrite(DIRx, HIGH);
  digitalWrite(ENAx, LOW);
  for (int i = 0; i < pulse; i++) { // Adjust the number of steps as needed
    digitalWrite(PULx, HIGH);
    delayMicroseconds(speed);
    digitalWrite(PULx, LOW);
    delayMicroseconds(speed);
  }
}

void moveRightX(int pulse, int speed) {
  digitalWrite(DIRx, LOW);
  digitalWrite(ENAx, LOW);
  for (int i = 0; i < pulse; i++) { // Adjust the number of steps as needed
    digitalWrite(PULx, HIGH);
    delayMicroseconds(speed);
    digitalWrite(PULx, LOW);
    delayMicroseconds(speed);
  }
}

void Ini_moveLeftY(int speed) {
  digitalWrite(DIRy, HIGH);
  digitalWrite(ENAy, LOW);
  digitalWrite(PULy, HIGH);
  delayMicroseconds(speed);
  digitalWrite(PULy, LOW);
  delayMicroseconds(speed);
}

void moveRightY(int pulse, int speed) {
  digitalWrite(DIRy, LOW);
  digitalWrite(ENAy, LOW);
  for (int i = 0; i <= pulse; i++) { // Adjust the number of steps as needed
    digitalWrite(PULy, HIGH);
    delayMicroseconds(speed);
    digitalWrite(PULy, LOW);
    delayMicroseconds(speed);
  }
}

void moveLeftY(int pulse, int speed) {
  digitalWrite(DIRy, HIGH);
  digitalWrite(ENAy, LOW);
  for (int i = 0; i <= pulse; i++) { 
    digitalWrite(PULy, HIGH);
    delayMicroseconds(speed);
    digitalWrite(PULy, LOW);
    delayMicroseconds(speed);
  }
}

void moveRightX_silent(int pulse, int speed){

        for(int k=1; k<=5; k++){
            moveRightX(2*distance,5*125.0/(k*valocity));
          } 
        moveRightX(20*distance,125.0/valocity);
        for(int k=5; k>0; k--){
            moveRightX(2*distance,5*125.0/(k*valocity));
        }

}
void moveLeftX_silent(int pulse, int speed){

        for(int k=1; k<=5; k++){
            moveLeftX(2*distance,5*125.0/(k*valocity));
          } 
        moveLeftX(20*distance,125.0/valocity);
        for(int k=5; k>0; k--){
            moveLeftX(2*distance,5*125.0/(k*valocity));
        }

}
void moveLeftY_silent(int pulse, int speed){

        for(int k=1; k<=5; k++){
            moveLeftY(2*distance,5*125.0/(k*valocity));
          } 
        moveLeftY(20*distance,125.0/valocity);
        for(int k=5; k>0; k--){
            moveLeftY(2*distance,5*125.0/(k*valocity));
        }

}
void moveRightY_silent(int pulse, int speed){
        for(int k=1; k<=5; k++){
            moveRightY(2*distance,5*125.0/(k*valocity));
          } 
        moveRightY(20*distance,125.0/valocity);
        for(int k=5; k>0; k--){
            moveRightY(2*distance,5*125.0/(k*valocity));
        }

}

