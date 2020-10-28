/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/vojen/Desktop/Studieting/IOT/Argon/IOT_project/IOTPlantWaterer/src/IOTPlantWaterer.ino"
/*
 * Project IOTPlantWaterer
 * Description:
 * Author:
 * Date:
 */


// setup() runs once, when the device is first turned on.
void setup();
void loop();
#line 10 "c:/Users/vojen/Desktop/Studieting/IOT/Argon/IOT_project/IOTPlantWaterer/src/IOTPlantWaterer.ino"
int MoistSense = A0; //Moistsensor / ADC 0
float analogvalue; // Here we are declaring the integer variable analogvalue, 
float bitres = 0.0008; // 100 microVolt
int bits = 4096; // The number of bits
float val = 0;
//which we will use later to store the value of the photoresistor or phototransistor.

void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(115200); // open serial port, set the baud rate as 9600 bps
  //setADCSampleTime(10);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  analogvalue = analogRead(MoistSense); //connect sensor to Analog 0
  analogvalue = analogvalue * bitres;
  
  Serial.println(analogvalue); //print the value to serial port
  delay(100);
}