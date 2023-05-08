#include <dht11.h>

// https://github.com/adidax/dht11#readme - DHT11 Liberary link

#define DHT11PIN 4



// Define the pin for the heat bed
#define HEATING_LED_PIN 12

// Define the target temperature for the heat bed
#define TARGET_TEMPERATURE 40

// Define the temperature range (in degrees Celsius) for turning the heat bed on and off
#define TEMPERATURE_RANGE 2

// Define the PID constants
#define KP .4
#define KI 0.2
#define KD 0.05

// Define the sample time (in milliseconds) for the PID controller
#define SAMPLE_TIME 1000

// Define variables for the PID controller
float setpoint = TARGET_TEMPERATURE;
float input, output;
double error, last_error = 0;
double ITerm, last_input = 0;
unsigned long last_time;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int ledPin = 16;  // 16 corresponds to GPIO16


dht11 DHT11;



void  setup()
{
  Serial.begin(9600);
  // Set the pin mode for the heat bed
  pinMode(HEATING_LED_PIN, OUTPUT);

  // Set the initial time for the PID controller
  last_time = millis();
 
}

void loop()
{
  Serial.println();

  int chk = DHT11.read(DHT11PIN);



  input = (float)DHT11.temperature;
  Serial.print("Current temperature: ");
  Serial.print(input);
  Serial.println("°C");



unsigned long now = millis();
  double time_change = (double)(now - last_time);
  if (time_change >= SAMPLE_TIME) {
    error = setpoint - input;
    ITerm += (KI * error * time_change) / 1000.0;
    if (ITerm > 255) ITerm = 255;
    else if (ITerm < 0) ITerm = 0;
    double dInput = (input - last_input) / time_change;
    output = KP * error + ITerm - KD * dInput;
    if (output > 255) output = 255;
    else if (output < 0) output = 0;
    last_input = input;
    last_error = error;
    last_time = now;
  }

  // Turn the heat bed on or off based on the PID output
  Serial.print("output : ");
  Serial.println(output);
  if (output < 1) {
    digitalWrite(HEATING_LED_PIN, HIGH);
    Serial.println("Heat bed off");
  } else {
    digitalWrite(HEATING_LED_PIN, LOW);
    Serial.println("Heat bed on");
  }

  // Wait for a moment before checking the temperature again
  delay(1000);

}
