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



STMPE1600DigiOut *xshutdown_top;
STMPE1600DigiOut *xshutdown_left;
STMPE1600DigiOut *xshutdown_right;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  xshutdown_top = new STMPE1600DigiOut(&Wire, GPIO_15, (0x42 * 2));
  xshutdown_left = new STMPE1600DigiOut(&Wire, GPIO_14, (0x43 * 2));
  xshutdown_right = new STMPE1600DigiOut(&Wire, GPIO_15, (0x43 * 2));
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

  sensor_top.setSignalRateLimit(0.1);
  sensor_top.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor_top.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor_left.setSignalRateLimit(0.1);
  sensor_left.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor_left.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  sensor_right.setSignalRateLimit(0.1);
  sensor_right.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor_right.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensor_top.setMeasurementTimingBudget(20000);
  sensor_left.setMeasurementTimingBudget(20000);
  sensor_right.setMeasurementTimingBudget(20000);
}

void loop()
{
  Serial.print(sensor_left.readRangeSingleMillimeters());
  if (sensor_left.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.print(" ");
  Serial.print(sensor_top.readRangeSingleMillimeters());
  if (sensor_top.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.print(" ");
  Serial.print(sensor_right.readRangeSingleMillimeters());
  if (sensor_right.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
