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



STMPE1600PinOut *xshutdown_top;
STMPE1600PinOut *xshutdown_left;
STMPE1600PinOut *xshutdown_right;

STMPE1600DigiOut *digit[4];

void printdigit(uint8_t digit){

  
}

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
}

void loop()
{
  Serial.print(sensor_left.readRangeContinuousMillimeters());
  if (sensor_left.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.print(" ");
  write_dec(sensor_top.readRangeContinuousMillimeters());
  Serial.print(" ");
  Serial.print(sensor_right.readRangeContinuousMillimeters());
  if (sensor_right.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
  Serial.print("time:");
  Serial.println(millis());
}
