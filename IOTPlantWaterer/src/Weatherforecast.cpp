/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/vojen/Desktop/Studieting/IOT/Argon/Weatherforecast/src/Weatherforecast.ino"
/*
 * Project Weatherforecast
 * Description:
 * Author:
 * Date:
 */

void myHandler(const char *event, const char *data);
void setup();
void sleep();
void loop();
void water(int temp);
#line 8 "c:/Users/vojen/Desktop/Studieting/IOT/Argon/Weatherforecast/src/Weatherforecast.ino"
#define HOOK_RESP "hook-response/TempHook"
#define HOOK_PUB "TempHook"
#define lessWet 3250
#define moreWet 2025
#define Wet 1625

// setup() runs once, when the device is first turned on.
int MoistSense = A0; //Moistsensor / ADC 0
int Relay = A1;      //Moistsensor / Digital out
int analogvalue;
bool debug = true;

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
      .duration(30s); //60 sec

  System.sleep(config);
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
    delay(60000);
  }
  else
  {
    Serial.println("Going to sleep");
    delay(35000);
    //sleep();
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

/*
{
    "event": "TempHook",
    "url": "https://api.openweathermap.org/data/2.5/onecall?",
    "requestType": "GET",
    "responseTemplate": "{{hourly.1.rain.1h}},{{hourly.2.rain.1h}},",
    "query": {
        "lat": "56.2316307",
        "lon": "10.2303381",
        "mode": "json",
        "exclude": "current,minutely,daily,alerts",
        "appid": "c6b64317b77368ea6e72a640612ed2b1"
    }
}
*/

/*

  JSONValue outerObj = JSONValue::parseCopy(data);
  //char temp = JSONType::type(data) ;

  JSONObjectIterator iter(outerObj);
  while (iter.next())
  {
    Log.info("key=%s value=%s",
             (const char *)iter.name(),
             (const char *)iter.value().toString());
  };

bool Weather::parse(String& data, weather_response_t& response) {
	/*
	 * example:
	 * http://api.openweathermap.org/data/2.5/forecast/daily?q=London,uk&units=metric&cnt=1
	 *
	 * {"cod":"200","message":0.3348,
	 * "city":{"id":2643743,"name":"London",
	 * "coord":{"lon":-0.12574,"lat":51.50853},
	 * "country":"GB","population":0,"sys":{"population":0}},"cnt":1,
	 * "list":[{"dt":1396267200,
	 * "temp":{"day":14.69,"min":8.6,"max":14.69,"night":8.6,"eve":14.69,"morn":14.69},
	 * "pressure":1019.37,"humidity":66,
	 * "weather":[{"id":803,"main":"Clouds","description":"broken clouds","icon":"04d"}],
	 * "speed":3.7,"deg":162,"clouds":64}]}
	 

	using namespace ArduinoJson::Parser;


	unsigned char buffer[data.length()];
	data.getBytes(buffer, sizeof(buffer), 0);
	JsonObject root = parser.parse((char*) buffer);
	if (!root.success()) {
		Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
		return false;
	}

	JsonValue daysList = root["list"];
	JsonValue today = daysList[0];

	response.temp_low = today["temp"]["min"];
	response.temp_high = today["temp"]["max"];

	JsonValue weather = today["weather"][0];
	response.descr = weather["description"];
	response.conditionCode = weather["id"];
		
	response.isSuccess = true;
	return true;
}
*/