/*
 * CrazyCars - CalibrateMotors
 * 
 * CrazyCars: Convert RC-cars to autonomous driving cars using an Arduino instead of the remote control. 
 * With this calibration tool you can adapt the RC-car motor controls values for the plattform.
 * @author: Bernhard Wallisch
 * 
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
#else
  #define TRACE(msg)
#endif //DEBUG
 
/*
 * LCD-Display:
 * 
 * The values are interactivly shown and set with the help of the LCD.
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
#define SELECT_BUTTON_PIN A1
#define PLUS_BUTTON_PIN A2
#define MINUS_BUTTON_PIN A3

bool isSelectButtonDown = false;
bool lastSelectButtonState = false;
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


#define SELECT_NONE 0
#define SELECT_MOTOR_STOP 1
#define SELECT_MOTOR_FORWARD_MAX 2
#define SELECT_MOTOR_REVERSE_MAX 3
#define SELECT_STEERING_CENTER 4
#define SELECT_STEERING_LEFT_MAX 5
#define SELECT_STEERING_RIGHT_MAX 6
#define SELECT_MAX 6
int select = SELECT_NONE;

const char* Names[][2] = {
  { "Car-Calibration ", "Press SELECT!   " },
  { "Motor stopped:  ", "" },
  { "Motor forward-  ", "max: " },
  { "Motor reverse-  ", "max: " },
  { "Steering center:", "" },
  { "Steering left-  ", "max: " },
  { "Steering right- ", "max: " },
};

#define VALUE_MIN 0
#define VALUE_MAX 255
int values[] = {
  0,
  90,
  180,
  0,
  90,
  0,
  180
};


void setup() {
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

  // Initialize Servos
  servoMotor.attach(SERVO_MOTOR_PIN); 
  servoSteering.attach(SERVO_STEERING_PIN);

  #ifdef LCD
    // Initialize LCD
    lcd.begin(16, 2); // 16 columns in 2 rows
  #endif //LCD

  TRACE( Names[select][0] );
  TRACE( Names[select][1] );
}


void loop() {
  // display 
  #ifdef LCD
    lcd.setCursor(0, 0);
    lcd.print( Names[select][0] );
    lcd.setCursor(0, 1);
    lcd.print( Names[select][1] );
    if( select > SELECT_NONE ) {
      lcd.setCursor(8, 1);
      lcd.print( F("    ") );
      lcd.setCursor(8, 1);
      lcd.print( String(values[select]) );
    }
  #endif //LCD
  
  // button control
  isSelectButtonDown = !digitalRead(SELECT_BUTTON_PIN);
  if( lastSelectButtonState != isSelectButtonDown ) {
    if( isSelectButtonDown ) {
      select++;
      if( select > SELECT_MAX )
        select = SELECT_NONE;

      TRACE( Names[select][0] );
      TRACE( Names[select][1] );
      #ifdef LCD
        lcd.clear();
      #endif //LCD
    }
    lastSelectButtonState = isSelectButtonDown;
  }
  
  isPlusButtonDown = !digitalRead(PLUS_BUTTON_PIN);
  if( select > SELECT_NONE && isPlusButtonDown ) {
    values[select]++;
    if( values[select] > VALUE_MAX )
      values[select] = VALUE_MAX;
  }
  
  isMinusButtonDown = !digitalRead(MINUS_BUTTON_PIN);
  if( select > SELECT_NONE && isMinusButtonDown ) {
    values[select]--;
    if( values[select] < VALUE_MIN )
      values[select] = VALUE_MIN;
  }

  // control the servos
  switch( select ) {
    case SELECT_MOTOR_STOP:
    case SELECT_MOTOR_FORWARD_MAX:
    case SELECT_MOTOR_REVERSE_MAX:
      servoMotor.write( values[select] );
      break;

    case SELECT_STEERING_CENTER:
    case SELECT_STEERING_LEFT_MAX:
    case SELECT_STEERING_RIGHT_MAX:
      servoSteering.write( values[select] );
      break;
  }

  TRACE( String(values[select]) );
  delay(100);
}
