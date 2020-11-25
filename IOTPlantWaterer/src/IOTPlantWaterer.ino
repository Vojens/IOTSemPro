/*
 * Project IOTPlantWaterer
 * Description:
 * Author:
 * Date:
 */


// setup() runs once, when the device is first turned on.
int MoistSense = A0; //Moistsensor / ADC 0
int Relay = A4;
float analogvalue; // Here we are declaring the integer variable analogvalue, 
float bitres = 0.0008; // 100 microVolt
int bits = 4096; // The number of bits
float val = 0;
//which we will use later to store the value of the photoresistor or phototransistor.

void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(115200); // open serial port, set the baud rate as 9600 bps
  //setADCSampleTime(10);
  pinMode(Relay, OUTPUT);

}

void sleep()
{
  SystemSleepConfiguration config;
  config.mode(SystemSleepMode::STOP).duration(30s); //60 sec
  System.sleep(config);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  analogvalue = analogRead(MoistSense); //connect sensor to Analog 0
  analogvalue = analogvalue * bitres;
  Serial.println(analogvalue); //print the value to serial port
  if(analogvalue > 2.6){
      digitalWrite(Relay, HIGH);
  }
  else if(analogvalue < 2.5 && analogvalue > 1.3){
      digitalWrite(Relay, LOW);
      Serial.println(" Normal humidity!!!!");
  }
  
  else if (analogvalue < 1.3){
            Serial.println("Very high humidity !!!!");
  }
  delay(1);
  
  //Serial.println("Going to sleep");
  //sleep();
  //Serial.println("I am now awake");
}

