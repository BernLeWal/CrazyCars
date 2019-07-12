/*
 * TestLaser
 * 
 * Test the Laser-Sensors within the CrazyCars Prototype Board
 * @author: Bernhard Wallisch
 * 
 * Using the IIC-bus driven Laser-Sensor Module GY-530 VL53L0X
 * Programming Sensor Address 0x30 and 0x31, working 2019-06-25
 */
#include "Adafruit_VL53L0X.h"

#define SHUT_LOX1 8
#define SHUT_LOX2 9

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();


void setup() {
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  // INIT SENSORS DISTANCE
  pinMode(SHUT_LOX1,OUTPUT);
  pinMode(SHUT_LOX2,OUTPUT);

  digitalWrite(SHUT_LOX1,LOW);
  digitalWrite(SHUT_LOX2,LOW);
  delay(10);
  digitalWrite(SHUT_LOX1,HIGH);
  digitalWrite(SHUT_LOX2,HIGH);
  delay(10);
  digitalWrite(SHUT_LOX2,LOW);
  
  Serial.println("Adafruit VL53L0X test S1(0x30)");
  if (!lox1.begin(0x30)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  digitalWrite(SHUT_LOX2,HIGH);
  Serial.println("Adafruit VL53L0X test S2(0x31)");
  if (!lox2.begin(0x31)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure1;
  VL53L0X_RangingMeasurementData_t measure2;
  
  Serial.print("Reading a measurement... ");
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  Serial.print("Distance1 (mm): "); 
  Serial.print(measure1.RangeMilliMeter);
  Serial.print("      Distance2 (mm): "); 
  Serial.println(measure2.RangeMilliMeter);
  //delay(100);
}
