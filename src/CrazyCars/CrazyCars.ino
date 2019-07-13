/*
 * CrazyCars - 
 * 
 * Convert RC-cars to autonomous driving cars using an Arduino instead of the remote control. 
 * @author: Bernhard Wallisch
 * 
 * Using the IIC-bus driven Laser-Sensor Module GY-530 VL53L0X
 * 
 * ATTENTION: Arduino Uno R3 has too little RAM (and Flash) storage - it only supports one sensor!
 */

/////////////////////////////////////////////////////////////////////////

/*
 * Debugging-Features:
 * 
 * Enable Debugging output on the Serial-Monitor with defining DEBUG.
 */
//#define DEBUG

#ifdef DEBUG
  #define TRACE(msg) Serial.println(msg)
  #define TRACEF(msg) Serial.println(F(msg))
#else
  #define TRACE(msg)
  #define TRACEF(msg)
#endif //DEBUG

/**
 * Time-of-Flight Laser Sensor: GY530 VL53L0X
 */
#include "Adafruit_VL53L0X.h"   // for GY530 VL53L0X

#define LOX_LEFT_SHUTDOWN 8
#define LOX_RIGHT_SHUTDOWN 9
#define LOX_FORWARD_SHUTDOWN 6
#define LOX_REVERSE_SHUTDOWN 7

//Adafruit_VL53L0X loxLeft = Adafruit_VL53L0X();
//Adafruit_VL53L0X loxRight = Adafruit_VL53L0X();
Adafruit_VL53L0X loxForward = Adafruit_VL53L0X();
//Adafruit_VL53L0X loxReverse = Adafruit_VL53L0X();

/*
 * LCD Display: model 1602 directly connected (without I2C)
 * 
 * If you don't use a LCD the comment out the following line. (use DEBUG instead)
 */
#define LCD

#ifdef LCD
  #include <LiquidCrystal.h>      // for 1602 LCD
  LiquidCrystal lcd(13, 12, 5, 4, 3, 2); 
#endif //LCD


/*
 * Buttons:
 */
#define MODE_BUTTON_PIN A0
#define SELECT_BUTTON_PIN A1
#define PLUS_BUTTON_PIN A2
#define MINUS_BUTTON_PIN A3

bool isModeButtonDown = false;
bool lastModeButtonState = false; // used for button-pressed handling
bool isSelectButtonDown = false;
bool lastSelectButtonState = false; // used for button-pressed handling
bool isPlusButtonDown = false;
bool isMinusButtonDown = false;

/*
 * Servos:
 */
#include <Servo.h> 

#define SERVO_MOTOR_PIN 10
#define SERVO_STEERING_PIN 11

Servo servoMotor; 
Servo servoSteering; 




#define MODE_NONE 0
#define MODE_INIT 1
#define MODE_STOP 2
#define MODE_GO 3
int mode = MODE_NONE;

const char* ModeNames[] = {
  "NONE", "INIT", "STOP", " GO "
};

void setup() {
  mode = MODE_INIT;
  
  // Initialize pins for buttons
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MINUS_BUTTON_PIN, INPUT_PULLUP);

  #ifdef DEBUG
    // Initialize Serial Monitor (for debugging purposes)
    Serial.begin(115200);
    // wait until serial port opens for native USB devices
    while (! Serial) {
      delay(1);
    }
  #endif //DEBUG

  #ifdef LCD
    // Initialize LCD
    lcd.begin(16, 2); // 16 columns in 2 rows
    lcd.setCursor(0, 0);
    lcd.print("CrazyCars V1.01");
    TRACEF("CrazyCars V1.01");
    lcd.setCursor(0, 1);
    lcd.print(ModeNames[mode]);
    TRACE(ModeNames[mode]);
  #endif LCD 

  // Initialize Time-of-Flight Laser-Sensors
  TRACEF("initializing Time-of-Flight Laser-Sensors");
  pinMode(LOX_LEFT_SHUTDOWN,OUTPUT);
  pinMode(LOX_RIGHT_SHUTDOWN,OUTPUT);
  pinMode(LOX_FORWARD_SHUTDOWN,OUTPUT);
  pinMode(LOX_REVERSE_SHUTDOWN,OUTPUT);

  // Reset all Sensors
  TRACEF("reseting all Sensors");
  digitalWrite(LOX_LEFT_SHUTDOWN,LOW);
  digitalWrite(LOX_RIGHT_SHUTDOWN,LOW);
  digitalWrite(LOX_FORWARD_SHUTDOWN,LOW);
  digitalWrite(LOX_REVERSE_SHUTDOWN,LOW);
  delay(10);
  digitalWrite(LOX_LEFT_SHUTDOWN,HIGH);
  digitalWrite(LOX_RIGHT_SHUTDOWN,HIGH);
  digitalWrite(LOX_FORWARD_SHUTDOWN,HIGH);
  digitalWrite(LOX_REVERSE_SHUTDOWN,HIGH);
  delay(10);
  digitalWrite(LOX_RIGHT_SHUTDOWN,LOW);
  digitalWrite(LOX_LEFT_SHUTDOWN,LOW);
  digitalWrite(LOX_REVERSE_SHUTDOWN,LOW);
  delay(10);

  TRACEF("configure FORWARD sensor to Address 0x32");
  if (!loxForward.begin(0x32)) {
    TRACEF("Failed to boot FORWARD sensor");
    //while(1);
  }
  delay(10);

  digitalWrite(LOX_RIGHT_SHUTDOWN,HIGH);
//  TRACEF("configure RIGHT sensor to Address 0x31");
//  if (!loxRight.begin(0x31)) {
//    TRACEF("Failed to boot RIGHT sensor");
//    //while(1);
//  }
  delay(10);

  digitalWrite(LOX_LEFT_SHUTDOWN,HIGH);
//  TRACEF("configure LEFT sensor to Address 0x30");
//  if (!loxLeft.begin(0x30)) {
//    TRACEF("Failed to boot LEFT sensor");
//    //while(1);
//  }
  delay(10);

  digitalWrite(LOX_REVERSE_SHUTDOWN,HIGH);
//  TRACEF("configure REVERSE sensor to Address 0x33");
//  if (!loxReverse.begin(0x33)) {
//    TRACEF("Failed to boot REVERSE sensor");
//    //while(1);
//  }
  delay(10);

  // Initialize Servos
  servoMotor.attach(SERVO_MOTOR_PIN); 
  servoSteering.attach(SERVO_STEERING_PIN);

  TRACEF("INIT done.\n"); 
  #ifdef LCD
    lcd.clear();
  #endif LCD 
  mode = MODE_STOP;
}


float driveFunction(int distanceMM) {
  // approximation of y=k*x + d where y is limited between 0.0 and 1.0
  float result = /* 1.0* */ distanceMM - 0.25;
  if ( result > 1.0 )
    return 1.0; // maximum
  else if ( result < 0.0 )
    return 0.0; // minimum
  return result;
}

void loop() {
  // Measure distances:
  VL53L0X_RangingMeasurementData_t left;
  VL53L0X_RangingMeasurementData_t right;
  VL53L0X_RangingMeasurementData_t forward;
  VL53L0X_RangingMeasurementData_t reverse;
  
  loxForward.rangingTest(&forward, false); // pass in 'true' to get debug data printout!
//  loxLeft.rangingTest(&left, false); // pass in 'true' to get debug data printout!
//  loxRight.rangingTest(&right, false); // pass in 'true' to get debug data printout!
//  loxReverse.rangingTest(&reverse, false); // pass in 'true' to get debug data printout!

  // Display output:
  #ifdef LCD
    lcd.setCursor(0, 0);
    lcd.print(String( ((float)left.RangeMilliMeter) / 1000.0, 2));
    lcd.setCursor(6, 0);
    lcd.print(String( ((float)forward.RangeMilliMeter) / 1000.0, 2));
    lcd.setCursor(12, 0);
    lcd.print(String( ((float)right.RangeMilliMeter) / 1000.0, 2));
    lcd.setCursor(6, 1);
    lcd.print(String( ((float)reverse.RangeMilliMeter) / 1000.0, 2));
    lcd.setCursor(0, 1);
    lcd.print(ModeNames[mode]);
  #endif //LCD

  #ifdef DEBUG
    Serial.print(F("Left (mm): ")); 
    Serial.print(left.RangeMilliMeter);
    Serial.print(F("   Forward (mm): ")); 
    Serial.print(forward.RangeMilliMeter);
    Serial.print(F("   Right (mm): ")); 
    Serial.print(right.RangeMilliMeter);
    Serial.print(F("   Reverse (mm): ")); 
    Serial.print(reverse.RangeMilliMeter);
    Serial.println();
  #endif //DEBUG

  // Button control:
  isModeButtonDown = !digitalRead(MODE_BUTTON_PIN);
  if( lastModeButtonState != isModeButtonDown ) {
    if( isModeButtonDown ) {
      if( mode == MODE_STOP )
        mode = MODE_GO;
      else
        mode = MODE_STOP;
    }
    lastModeButtonState = isModeButtonDown;
  }
  
  //delay(100);
}
