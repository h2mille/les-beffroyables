#include "Adafruit_VL53L0X.h"
#include "stmpe1600_class.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

STMPE1600DigiOut *xshutdown_top;
STMPE1600DigiOut *xshutdown_left;
STMPE1600DigiOut *xshutdown_right;

void setup() {
  Serial.begin(115200);
  xshutdown_top = new STMPE1600DigiOut(&Wire, GPIO_15, (0x42 * 2));
  xshutdown_left = new STMPE1600DigiOut(&Wire, GPIO_14, (0x43 * 2));
  xshutdown_right = new STMPE1600DigiOut(&Wire, GPIO_15, (0x43 * 2));
  xshutdown_top->write(HIGH);
  xshutdown_left->write(LOW);
  xshutdown_right->write(LOW);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("MeasurementTimeUsec: "); Serial.println(measure.MeasurementTimeUsec);
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    Serial.print("RangeDMaxMilliMeter (mm): "); Serial.println(measure.RangeDMaxMilliMeter);
    Serial.print("EffectiveSpadRtnCount: "); Serial.println(measure.EffectiveSpadRtnCount/256);
    Serial.print("ZoneId: "); Serial.println(measure.ZoneId);
    Serial.print("RangeFractionalPart: "); Serial.println(measure.RangeFractionalPart);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
}
