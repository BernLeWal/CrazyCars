/*
 * CrazyCars - 
 * 
 * Convert RC-cars to autonomous driving cars using an Arduino instead of the remote control. 
 * @author: Bernhard Wallisch
 * 
 * Using the IIC-bus driven Laser-Sensor Module GY-530 VL53L0X
 */

/////////////////////////////////////////////////////////////////////////

/*
 * Debugging-Features:
 * 
 * Enable Debugging output on the Serial-Monitor with defining DEBUG.
 */
#define DEBUG

#ifdef DEBUG
  #define TRACE(msg) Serial.println(F(msg))
#else
  #define TRACE(msg)
#endif //DEBUG
 
#include "Adafruit_VL53L0X.h"   // for GY530 VL53L0X

#define LOX_LEFT_SHUTDOWN 8
#define LOX_RIGHT_SHUTDOWN 9
Adafruit_VL53L0X loxLeft = Adafruit_VL53L0X();
Adafruit_VL53L0X loxRight = Adafruit_VL53L0X();

/*
 * LCD Display:
 * 
 * Enable by defining LCD
 */
#define LCD

#ifdef LCD
  #include <LiquidCrystal.h>      // for 1602 LCD
  LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 
#endif //LCD

void setup() {

  #ifdef DEBUG
    // Initialize Serial Monitor (for debugging purposes)
    Serial.begin(115200);
    // wait until serial port opens for native USB devices
    while (! Serial) {
      delay(1);
    }
  #endif //DEBUG

  // Initialize Time-of-Flight Laser-Sensors
  TRACE("initializing Time-of-Flight Laser-Sensors");
  pinMode(LOX_LEFT_SHUTDOWN,OUTPUT);
  pinMode(LOX_RIGHT_SHUTDOWN,OUTPUT);

  // Reset all Sensors
  TRACE("reseting all Sensors");
  digitalWrite(LOX_LEFT_SHUTDOWN,LOW);
  digitalWrite(LOX_RIGHT_SHUTDOWN,LOW);
  delay(10);
  digitalWrite(LOX_LEFT_SHUTDOWN,HIGH);
  digitalWrite(LOX_RIGHT_SHUTDOWN,HIGH);
  delay(10);
  digitalWrite(LOX_RIGHT_SHUTDOWN,LOW);

  TRACE("configure LEFT sensor to Address 0x30");
  if (!loxLeft.begin(0x30)) {
    TRACE("Failed to boot VL53L0X");
    //while(1);
  }

  digitalWrite(LOX_RIGHT_SHUTDOWN,HIGH);
  TRACE("configure RIGHT sensor to Address 0x31");
  if (!loxRight.begin(0x31)) {
    TRACE("Failed to boot VL53L0X");
    //while(1);
  }

  #ifdef LCD
    // Initialize LCD
    lcd.begin(16, 2); // 16 columns in 2 rows
/*    lcd.setCursor(0, 0);
    lcd.print("CrazyCars V1.01");
    Serial.println(F("CrazyCars V1.01"));
    lcd.setCursor(0, 1);
    lcd.print(F("INIT"));
    Serial.println(F("INIT..."));
*/ 
  //  lcd.clear();
  #endif LCD 

  TRACE("INIT done.\n"); 
}

void loop() {
  VL53L0X_RangingMeasurementData_t left;
  VL53L0X_RangingMeasurementData_t right;
  
  TRACE("Reading a measurement... ");
  loxLeft.rangingTest(&left, false); // pass in 'true' to get debug data printout!
  loxRight.rangingTest(&right, false); // pass in 'true' to get debug data printout!

  #ifdef LCD
    lcd.setCursor(0, 0);
    lcd.print(String( ((float)left.RangeMilliMeter) / 1000.0, 2));
    lcd.setCursor(12, 0);
    lcd.print(String( ((float)right.RangeMilliMeter) / 1000.0, 2));
    lcd.setCursor(0, 1);
    lcd.print("STOP");
  #endif //LCD

  #ifdef DEBUG
    Serial.print("Left (mm): "); 
    Serial.print(left.RangeMilliMeter);
    Serial.print("   Right (mm): "); 
    Serial.println(right.RangeMilliMeter);
  #endif //DEBUG
  //delay(100);
}
