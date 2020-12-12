/*
 * Project IOT Plant Waterer
 * Description: Waters plants depending on forecast
 * Author: Mevlüt & Tommy
 * Date: 121220
 */

#define HOOK_RESP "hook-response/TempHook"
#define HOOK_PUB "TempHook"
#define lessWet 3250
#define moreWet 2025
#define Wet 1625

// setup() runs once, when the device is first turned on.
int MoistSense = A0; //Moistsensor / ADC 0
int Relay = A4;
float analogvalue; // Here we are declaring the integer variable analogvalue,
//float bitres = 0.0008; // 100 microVolt
//int bits = 4096;       // The number of bits
void water(int);

void myHandler(const char *event, const char *data)
{
  String temp = data;
  Serial.println("Data fra web");
  Serial.println(temp);

  Serial.println("data length: ");
  Serial.println(temp.length());
  if (temp.length() > 2)
  {
    Serial.println("It's gonna rain!");
  }
  else
  {
    Serial.println("No rain in sight!");
    water(moreWet);
  };
}

// setup() runs once, when the device is first turned on.
void setup()
{
  // Put initialization like pinMode and begin functions here.
  Particle.subscribe(HOOK_RESP, myHandler, MY_DEVICES);
  //Particle.subscribe("jsonTest", myHandler);
  Serial.begin(115200);
}

// loop() runs over and over again, as quickly as it can execute.
void sleep()
{
  SystemSleepConfiguration config;
  config.mode(SystemSleepMode::STOP)
      .duration(30s); //60 sec

  System.sleep(config);
  //Particle.publish(HOOK_PUB, PRIVATE);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // The core of your code will likely live here.
  analogvalue = analogRead(MoistSense); //connect sensor to Analog 0

  //analogvalue = analogvalue * bitres;
  Serial.println(analogvalue); //print the value to serial port

  // Dry
  // 2,6V || 3250
  if (analogvalue > lessWet)
  {
    Serial.println(analogvalue);
    digitalWrite(Relay, HIGH);
    Serial.println("Starting to water!");
    // Emergency watering
    // 2,6V || 3250
    while (analogvalue > lessWet)
    {
      analogvalue = analogRead(MoistSense);
      Serial.println(analogvalue);
      //String temp = analogvalue;
    }
    Serial.println("Stopped watering!");
    digitalWrite(Relay, LOW);
  }

  // Less wet
  // 2,6V && 1,62V || 3250 && 2025
  else if (analogvalue < lessWet && analogvalue > moreWet)
  {
    Particle.publish(HOOK_PUB, PRIVATE);
    Serial.println(" Lesswet humidity!!!!");
  }

  // More wet
  // 1,62V && 1,3V || 2025 && 1625
  else if (analogvalue < lessWet && analogvalue > Wet)
  {
    Particle.publish(HOOK_PUB, PRIVATE);
    Serial.println(" MoreWet humidity!!!!");
  }
  // Wet
  // 1,3V || 1625
  else if (analogvalue < Wet)
  {
    Serial.println("Very high humidity !!!!");
  }
  delay(60000);

  //Serial.println("Going to sleep");
  //sleep();
  //Serial.println("I am now awake");
}

void water(int temp)
{
  digitalWrite(Relay, HIGH);
  Serial.println("Starting to water!");
  // 2,6V || 3250
  while (analogvalue > temp)
  {
    analogvalue = analogRead(MoistSense);
    Serial.println(analogvalue);
    //String temp = analogvalue;
  }
  Serial.println("Stopped watering!");
  digitalWrite(Relay, LOW);
};

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

