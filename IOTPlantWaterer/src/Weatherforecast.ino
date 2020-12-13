/*
 * Project IOT plant waterer
 * Description: IOT plant waterer
 * Author: Mevlüt & Tommy
 */

// Important stuff
#define HOOK_RESP "hook-response/TempHook"
#define HOOK_PUB "TempHook"
#define lessWet 3250 // Low limit
#define moreWet 2025 // a bit higher limit
#define Wet 1625     // Soaking limit
int MoistSense = A0; //  Moistsensor / ADC 0
int Relay = A1;      //  Moistsensor / Digital out
int analogvalue;
bool debug = true; // If we wanna debug

//float bitres = 0.0008; // 100 microVolt
//int bits = 4096;       // The number of bits

// Prototype for watering
void water(int);

// Incoming messagehandler
void myHandler(const char *event, const char *data)
{
  String temp = data;
  Serial.println("Data fra web");
  Serial.println(temp);
  Serial.println("data length: ");
  Serial.println(temp.length());
  // If message is only commas, theres no rain in sight
  if (temp.length() > 2)
  {
    Serial.println("It's gonna rain!");
  }
  else
  {
    if (analogvalue < lessWet && analogvalue > moreWet)
    {
      Serial.println("No rain in sight!");
      water(moreWet);
    }
    else
    {
      Serial.println("No rain in sight!");
      Serial.println("No worries, it's wet enough");
    };
  };
}

// setup() runs once, when the device is first turned on.
void setup()
{
  // Put initialization like pinMode and begin functions here.
  Particle.subscribe(HOOK_RESP, myHandler, MY_DEVICES);
  pinMode(Relay, OUTPUT);
  Serial.begin(115200);
}

// loop() runs over and over again, as quickly as it can execute.
void sleep()
{
  SystemSleepConfiguration config;
  config.mode(SystemSleepMode::STOP)
      .duration(30s); //30 sec
  System.sleep(config);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // Get sensor readout
  analogvalue = analogRead(MoistSense);
  //analogvalue = analogvalue * bitres;
  Serial.println(analogvalue);

  // Dry
  // 2,6V || 3250
  if (analogvalue > lessWet)
  {
    Serial.println(analogvalue);
    water(lessWet);
  }

  // Less wet
  // 2,6V && 1,62V || 3250 && 2025
  else if (analogvalue < lessWet && analogvalue > moreWet)
  {
    if (!debug)
    {
      Particle.publish(HOOK_PUB, PRIVATE);
    };
    Serial.println(" Lesswet humidity!!!!");
  }

  // More wet
  // 1,62V && 1,3V || 2025 && 1625
  else if (analogvalue < moreWet && analogvalue > Wet)
  {
    if (!debug)
    {
      Particle.publish(HOOK_PUB, PRIVATE);
    };
    Serial.println(" MoreWet humidity!!!!");
  }
  // Wet
  // 1,3V || 1625
  else if (analogvalue < Wet)
  {
    Serial.println("Very high humidity !!!!");
  }

  Serial.println("Delaying");
  if (debug)
  {
    delay(30000);
  }
  else
  {
    Serial.println("Going to sleep");
    sleep();
    Serial.println("I am now awake");
  };
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
  }
  Serial.println("Stopped watering!");
  digitalWrite(Relay, LOW);
};

// Particle CLI command for sending rain data
// particle publish hook-response/TempHook '1.20,,'

