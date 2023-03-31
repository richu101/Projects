// Define the pin for the temperature sensor
#define TEMP_SENSOR_PIN A0

// Define the pin for the heat bed
#define HEATBED_PIN 16

// Define the target temperature for the heat bed
#define TARGET_TEMPERATURE 60

// Define the temperature range (in degrees Celsius) for turning the heat bed on and off
#define TEMPERATURE_RANGE 2

// Define the PID constants
#define KP 5.0
#define KI 0.5
#define KD 1.0

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


void setup() {

    // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  // Start serial communication
  Serial.begin(9600);

  // Set the pin mode for the heat bed
  pinMode(HEATBED_PIN, OUTPUT);

  // Set the initial time for the PID controller
  last_time = millis();
}

void loop() {
  // Read the analog value from the temperature sensor and convert it to degrees Celsius
  int sensorValue = analogRead(TEMP_SENSOR_PIN);
  input = (sensorValue * 5.0 / 1023.0 - 0.5) * 100.0;

  // Print the current temperature to the serial monitor
  Serial.print("Current temperature: ");
  Serial.print(input);
  Serial.println("°C");

  // Calculate the error and update the PID controller
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
  if (output < 1) {
    digitalWrite(HEATBED_PIN, LOW);
    Serial.println("Heat bed off");
  } else {
    digitalWrite(HEATBED_PIN, HIGH);
    Serial.println("Heat bed on");
  }
  // analog write for arduino boards
  // analogWrite(HEATBED_PIN, output);
ledcWrite(ledChannel, output);
  // Wait for a moment before checking the temperature again
  delay(1000);
}
