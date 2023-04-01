#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>
Adafruit_BMP280 bme; // I2C

#include"sensors.h"

    

float Inputpara :: getdistance(const int trig, const int echo)
{   
  
 digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor

    return (distance);

}



bool Inputpara :: getFireState(const int firepin)
{
  pinMode(firepin, INPUT);
  bool Fstate = digitalRead(firepin);
  return(!Fstate);
}
long Inputpara :: getvibration(const int vs){
  long measurement=pulseIn (vs, LOW);  //wait for the pin to get HIGH and returns measurement
  return measurement/1000;
}
void Inputpara :: bemBegin(){

if (!bme.begin(0x76)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
}  

}

float Inputpara :: getpressure(){

  return(bme.readPressure());
  
}
float Inputpara :: getsmock(int smokeA0){
  return(analogRead(smokeA0));

}

float Inputpara :: getaltitude(){

  return(bme.readAltitude(1013.25));
  
}

int Inputpara :: getgasamount(const int gasSensorr)
{
  return(analogRead(gasSensorr));
}

bool Inputpara :: getMicrowave(const int mpin)
{
  bool val = digitalRead(mpin); //Read Pin as input
  bool flg;
     if((val == 1) && (flg==0))
     {
        
        Serial.println("Motion Detected");
        flg = 1;
     }
     if(val == 0)
     {
        
        Serial.println("NO Motion"); 
        flg = 0;
     } 
     return(val);
}
