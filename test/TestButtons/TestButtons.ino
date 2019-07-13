/*
 * TestButtons
 * 
 * Test the Buttons within the CrazyCars Prototype Board
 * @author: Bernhard Wallisch
 * 
 * The internal pull-up resistors are used for the four buttons which are
 * connected between A0-A3 and GND. 
 * PIN 13 is used for LED output toggled by click on the MODE-Button on A0:
 */

#define MODE_BUTTON_PIN A0
#define SELECT_BUTTON_PIN A1
#define PLUS_BUTTON_PIN A2
#define MINUS_BUTTON_PIN A3

#define LED_PIN 13

bool isModeButtonDown = false;
bool lastModeButtonState = false;
bool isSelectButtonDown = false;
bool isPlusButtonDown = false;
bool isMinusButtonDown = false;

int ledState = LOW;

void setup() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MINUS_BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }  
}

void loop() {

  isModeButtonDown = !digitalRead(MODE_BUTTON_PIN);
  if( lastModeButtonState != isModeButtonDown ) {
    if( isModeButtonDown ) {
      // toggle LED
      ledState = ( ledState==LOW ) ? HIGH : LOW;
      digitalWrite(LED_PIN, ledState);
    }
    lastModeButtonState = isModeButtonDown;
  }
  isSelectButtonDown = !digitalRead(SELECT_BUTTON_PIN);
  isPlusButtonDown = !digitalRead(PLUS_BUTTON_PIN);
  isMinusButtonDown = !digitalRead(MINUS_BUTTON_PIN);
  
  Serial.print(F("Buttons: "));
  Serial.print( isModeButtonDown ? F("MODE ") : F("mode ") );
  Serial.print( isSelectButtonDown ? F("SELECT ") : F("select ") );
  Serial.print( isPlusButtonDown ? F("PLUS ") : F("plus ") );
  Serial.println( isMinusButtonDown ? F("MINUS ") : F("minus ") );
}
