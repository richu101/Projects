#include<Arduino.h>




  


typedef struct
{
  static float distance;
  static float alchohole;
  
  
}sensor_val;



class Inputpara
{
    private:
    uint32_t delayMS;
    float distance;
    long duration;
    float k = -0.1;  //slope value
    float c = 38.1;  //calibration value

    public:

    float getdistance(const int trig, const int echo);
    bool getFireState(const int firepin);
    long  getvibration(const int vs);
    void bemBegin();
    float getpressure();
    float getaltitude();
    float getsmock(int smokeA0);
    int getgasamount(const int gasSensorr);
    bool getMicrowave(const int mpin);
};
