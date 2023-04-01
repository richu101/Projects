#include"sensors.h"

Inputpara sens;

#define echo1 4  // ultrasonic
#define trig1 24  // ultrasonic
#define echo2 5 // ultrasonic
#define trig2 22  // ultrasonic
#define firepinn 6 // fire sensor
#define vibpin 7 // D0
#define DHTPIN 2 //  OUT
#define relay1 8 
#define relay2 9
#define relay3 10
#define relay4 13
#define gaspin A1 //MQ-2
#define smcokpin A0 //MQ135 Smock sensor A0
#define microvavepin 3// microvave sensor pin
#define BLYNK_TEMPLATE_ID           "TMPLWkE7hPV4"
#define BLYNK_DEVICE_NAME           "PRADHIRODHA"
#define BLYNK_AUTH_TOKEN            "W-jiyPZNipyjtyNH0zLEv1qyYxGWwrVW"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "tech rick io";
char pass[] = "techrick";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define ESP_Slave_ADDR 0x08
#define DHTPIN 2  
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

static float temprature ;
static float humidity;
static float distancee1;
static float distancee2;
static float vibration;
static bool firestate,humandetection;
double pressure ;
int altitudee,Gasval,Smockval ;
static float smock = 0;

uint32_t delayMS;

#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

BlynkTimer timer;
void getdht();
void myTimerEvent();
void DhtInit();

BLYNK_WRITE(V8)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(relay1,pinValue);
  // process received value
}

BLYNK_WRITE(V9)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(relay2,pinValue);
  // process received value
}

BLYNK_WRITE(V10)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(relay3,pinValue);
  // process received value
}

BLYNK_WRITE(V11)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(relay4,pinValue);
  // process received value
}


void setup() {
  
Serial.begin(115200);
EspSerial.begin(ESP8266_BAUD);
sens.bemBegin();
delay(10);
Blynk.begin(auth, wifi, ssid, pass);
dht.begin();
void DhtInit();  
pinMode(trig1, OUTPUT); 
pinMode(echo1, INPUT); 
pinMode(trig2, OUTPUT); 
pinMode(echo2, INPUT); 
pinMode(firepinn, INPUT);
pinMode(vibpin, INPUT);
pinMode(relay1, OUTPUT);
 pinMode(relay2, OUTPUT);
 pinMode(relay3, OUTPUT);
 pinMode(relay4, OUTPUT);
timer.setInterval(1000L, myTimerEvent);
Serial.println("Void Setup Done");
}

void loop() {

 Blynk.run();
 timer.run();

}

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  getdht();
  firestate = sens.getFireState(firepinn);
  distancee1 = sens.getdistance(trig1,echo1); //echo - trig
  distancee2 = sens.getdistance(trig2,echo2);
  vibration = sens.getvibration(vibpin);
  pressure = sens.getpressure();
  altitudee = sens.getaltitude();
  Smockval = sens.getsmock(smcokpin);
  Gasval = sens.getgasamount(gaspin);
  humandetection = sens.getMicrowave(microvavepin);  
  Serial.print("Fire state : ");
  Serial.println(firestate);  
  Serial.print("vibration value : ");
  Serial.println(vibration);
  Serial.print("Pressure : ");
  Serial.println(pressure);
  Serial.print("Altitude : ");
  Serial.println(altitudee);
  Serial.print("Smock Value : ");
  Serial.println(Smockval);
  Serial.print("Gas Amount : ");
  Serial.println(Gasval);
  Serial.print("Distance 1 = ");
  Serial.println(distancee1);
  Serial.print("Distance 2 = ");
  Serial.println(distancee2);

// ----------------------------------------------------------------------------------------------------------

  Blynk.virtualWrite(V4, vibration);
  Blynk.virtualWrite(V12, firestate);
  Blynk.virtualWrite(V0, humidity);
  Blynk.virtualWrite(V1, temprature);
  Blynk.virtualWrite(V3,  distancee1);
  Blynk.virtualWrite(V5,  distancee2);
  Blynk.virtualWrite(V6, pressure);
  Blynk.virtualWrite(V7, altitudee);
  Blynk.virtualWrite(V15, Gasval);
  Blynk.virtualWrite(V14, Smockval);  
  Blynk.virtualWrite(V13, humandetection);
}


void DhtInit()
{
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

}


void getdht()
{
  

  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
   temprature = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(temprature);
    Serial.println(F("°C"));
  } 
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    humidity = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F("%"));
  }

}
