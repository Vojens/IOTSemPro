/*
 * Project IOTPlantWaterer
 * Description:
 * Author:
 * Date:
 */
#define HOOK_RESP "hook-response/TempHook"
#define HOOK_PUB "TempHook"

// setup() runs once, when the device is first turned on.
int MoistSense = A0; //Moistsensor / ADC 0
int Relay = A4;
float analogvalue; // Here we are declaring the integer variable analogvalue, 
float bitres = 0.0008; // 100 microVolt
int bits = 4096; // The number of bits
float val = 0;
//which we will use later to store the value of the photoresistor or phototransistor.

void myHandler(const char *event, const char *data)
{
  // Handle the integration response
  // If 2 commas are returned there'll be no rain for the next 2 hours.
  // Response template at the bottom.
  Serial.println(event);
  Serial.println(data);  
}

void setup() {
  // Subscribing to our wenhook
  Particle.subscribe(HOOK_RESP, myHandler, MY_DEVICES);
  //Particle.subscribe("jsonTest", myHandler);
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
  Particle.publish(HOOK_PUB, data, PRIVATE);
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

/*
{
    "event": "TempHook",
    "url": "https://api.openweathermap.org/data/2.5/onecall?",
    "requestType": "POST",
    "noDefaults": true,
    "rejectUnauthorized": false,
    "responseTemplate": "{{hourly.1.rain.1h}},{{hourly.2.rain.1h}},",
    "query": {
        "lat": "56.2316307",
        "lon": "10.2303381",
        "mode": "json",
        "exclude": "current,minutely,daily,alerts",
        "appid": "apikey"
    }
}
*/

