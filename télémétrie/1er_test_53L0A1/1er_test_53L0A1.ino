/* This example shows how to get single-shot range
 measurements from the VL53L0X. The sensor can optionally be
 configured with different ranging profiles, as described in
 the VL53L0X API user manual, to get better performance for
 a certain application. This code is based on the four
 "SingleRanging" examples in the VL53L0X API.

 The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>
#include "stmpe1600_class.h"

VL53L0X sensor_top;
VL53L0X sensor_left;
VL53L0X sensor_right;


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE

#define NO_DETECT_THRETHOLD 5
#define DETECT_THRETHOLD 3

STMPE1600PinOut *xshutdown_top;
STMPE1600PinOut *xshutdown_left;
STMPE1600PinOut *xshutdown_right;

STMPE1600DigiOut *digit[4];

int FIFO[3][10];
uint8_t FIFO_position[3]={0,0,0};
//uint8_t FIFO_coef[10]={10,8,6,5,4,3,2,1,1,1};
uint8_t FIFO_coef[10]  ={10,6,4,3,2,2,1,1,1,1};
uint8_t FIFO_sum;
uint8_t no_detect[3]={0,0,0};
uint8_t detect[3]={0,0,0};



void write_dec(int value)
{
  uint8_t i;
  for (i=0;i<4;i++)
  {
    digit[i]->write(value-(value/10)*10);
    value/=10;
  }
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  xshutdown_top = new STMPE1600PinOut(&Wire, GPIO_15, (0x42 * 2));
  xshutdown_left = new STMPE1600PinOut(&Wire, GPIO_14, (0x43 * 2));
  xshutdown_right = new STMPE1600PinOut(&Wire, GPIO_15, (0x43 * 2));

  digit[0]= new STMPE1600DigiOut(&Wire, 1, (0x42 * 2));
  digit[1]= new STMPE1600DigiOut(&Wire, 0, (0x42 * 2));
  digit[2]= new STMPE1600DigiOut(&Wire, 1, (0x43 * 2));
  digit[3]= new STMPE1600DigiOut(&Wire, 0, (0x43 * 2));



  xshutdown_top->write(HIGH);
  xshutdown_left->write(LOW);
  xshutdown_right->write(LOW);


  sensor_top.init();
  sensor_top.setAddress(0X10);
  sensor_top.setTimeout(500);
  xshutdown_left->write(HIGH);
  sensor_left.init();
  sensor_left.setAddress(0X12);
  sensor_left.setTimeout(500);
  xshutdown_right->write(HIGH);
  sensor_right.init();
  sensor_right.setAddress(0X14);
  sensor_right.setTimeout(500);

  sensor_top.setSignalRateLimit(0.25);
  sensor_top.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor_top.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor_left.setSignalRateLimit(0.25);
  sensor_left.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor_left.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor_right.setSignalRateLimit(0.25);
  sensor_right.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor_right.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensor_top.setMeasurementTimingBudget(33000);
  sensor_left.setMeasurementTimingBudget(33000);
  sensor_right.setMeasurementTimingBudget(33000);

  sensor_top.startContinuous();
  sensor_left.startContinuous();
  sensor_right.startContinuous();

  uint8_t i;
  for(i=0;i<3*10;i++)
    FIFO[0][i]=0;
  FIFO_sum=0;
  for(i=0;i<10;i++)
    FIFO_sum+=FIFO_coef[i];
  
}
void add_fifo(){
  int temp[3];
  temp[0]=sensor_left.readRangeContinuousMillimeters();
  temp[1]=sensor_top.readRangeContinuousMillimeters();
  temp[2]=sensor_right.readRangeContinuousMillimeters();
  uint8_t i;
  for(i=0;i<3;i++)
  {
    if(temp[i]>2500)
    {
      if(no_detect[i]<NO_DETECT_THRETHOLD)
        {
          no_detect[i]++;   
      }
      else
      {
        FIFO_position[i]= (FIFO_position[i]+1)%10;
        FIFO[i][FIFO_position[i]]= 2500;
        Serial.println("nothing detected for too long");
        detect[i]=0;
      }
    }
    else
    {
      if(detect[i]<DETECT_THRETHOLD)
      {
        detect[i]++;
      }
      else
      {    
        FIFO_position[i]= (FIFO_position[i]+1)%10;
        FIFO[i][FIFO_position[i]]= temp[i];
        no_detect[i]=0;
      }
    }
  }
}

void read_fifo(int32_t tab[]){
  uint8_t i,j;
  tab[0]=FIFO[0][FIFO_position[0]];tab[1]=FIFO[1][FIFO_position[1]];tab[2]=FIFO[2][FIFO_position[2]];
//  tab[0]=0;tab[1]=0;tab[2]=0;
//  for(j=0;j<3;j++){
////    Serial.println();
////    Serial.print(j);
////    Serial.print(" ");
//    for(i=0;i<10;i++)
//    {
////    Serial.print(FIFO[j][i]);
////    Serial.print("\t");
////    Serial.print(FIFO[j][i]*FIFO_coef[(FIFO_position[j]-i+10)%10]);
////    Serial.print("\t");
//      tab[j]+=(uint32_t)FIFO[j][i]*FIFO_coef[(FIFO_position[j]-i+10)%10];
//    }
//  }
//
//  tab[0]/=FIFO_sum;
//  tab[1]/=FIFO_sum;
//  tab[2]/=FIFO_sum;

}
void loop()
{
  int32_t tab[3];
  add_fifo();
  read_fifo(tab);
  write_dec(tab[1]);
  Serial.print(tab[0]);
  Serial.print(" ");
  Serial.print(tab[1]);
  Serial.print(" ");
  Serial.print(tab[2]);
  Serial.println();


}
