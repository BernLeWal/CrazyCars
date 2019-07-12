/*
 * CrazyCars - 
 * 
 * Convert RC-cars to autonomous driving cars using an Arduino instead of the remote control. 
 * @author: Bernhard Wallisch
 * 
 * Using the IIC-bus driven Laser-Sensor Module GY-530 VL53L0X
 */
 
#include "Adafruit_VL53L0X.h"   // for GY530 VL53L0X
#include <LiquidCrystal.h>      // for 1602 LCD

#define LOX_LEFT_SHUTDOWN 8
#define LOX_RIGHT_SHUTDOWN 9
Adafruit_VL53L0X loxLeft = Adafruit_VL53L0X();
Adafruit_VL53L0X loxRight = Adafruit_VL53L0X();

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 


void setup() {
  // Initialize Serial Monitor (for debugging purposes)
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  // Initialize Time-of-Flight Laser-Sensors
  Serial.println(F("initializing Time-of-Flight Laser-Sensors"));
  pinMode(LOX_LEFT_SHUTDOWN,OUTPUT);
  pinMode(LOX_RIGHT_SHUTDOWN,OUTPUT);

  // Reset all Sensors
  Serial.println(F("reseting all Sensors"));
  digitalWrite(LOX_LEFT_SHUTDOWN,LOW);
  digitalWrite(LOX_RIGHT_SHUTDOWN,LOW);
  delay(10);
  digitalWrite(LOX_LEFT_SHUTDOWN,HIGH);
  digitalWrite(LOX_RIGHT_SHUTDOWN,HIGH);
  delay(10);
  digitalWrite(LOX_RIGHT_SHUTDOWN,LOW);
  
  Serial.println(F("configure LEFT sensor to Address 0x30"));
  if (!loxLeft.begin(0x30)) {
    Serial.println(F("Failed to boot VL53L0X"));
    //while(1);
  }

  digitalWrite(LOX_RIGHT_SHUTDOWN,HIGH);
  Serial.println(F("configure RIGHT sensor to Address 0x31"));
  if (!loxRight.begin(0x31)) {
    Serial.println(F("Failed to boot VL53L0X"));
    //while(1);
  }
  
  // Initialize LCD
  lcd.begin(16, 2); // 16 columns in 2 rows
/*  lcd.setCursor(0, 0);
  lcd.print("CrazyCars V1.01");
  Serial.println(F("CrazyCars V1.01"));
  lcd.setCursor(0, 1);
  lcd.print(F("INIT"));
  Serial.println(F("INIT..."));
*/  
//  lcd.clear();

  Serial.println(F("INIT done.\n")); 
}

void loop() {
  VL53L0X_RangingMeasurementData_t left;
  VL53L0X_RangingMeasurementData_t right;
  
  Serial.print("Reading a measurement... ");
  loxLeft.rangingTest(&left, true); // pass in 'true' to get debug data printout!
  loxRight.rangingTest(&right, true); // pass in 'true' to get debug data printout!

  lcd.setCursor(0, 0);
  lcd.print(String( ((float)left.RangeMilliMeter) / 1000.0, 2));
  lcd.setCursor(12, 0);
  lcd.print(String( ((float)right.RangeMilliMeter) / 1000.0, 2));
  lcd.setCursor(0, 1);
  lcd.print("STOP");

  Serial.print("Left (mm): "); 
  Serial.print(left.RangeMilliMeter);
  Serial.print("   Right (mm): "); 
  Serial.println(right.RangeMilliMeter);
  //delay(100);
}
