/*
 * TestLCD
 * 
 * Test the LCD within the CrazyCars Prototype Board
 * @author: Bernhard Wallisch
 * 
 * LCD-Test sample is derived from https://funduino.de/nr-13-lcd-display
 * Changes to the original wiring see TestLCD_Wiring.png: 
 * - use Arduino Pin 6 (instead of 12) for LCD RW 
 * - use Arduino Pin 7 (instead of 13) for LCD E
 */

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 
float counter = 0.0;

void setup() 
{
  lcd.begin(16, 2); // 16 columns in 2 rows
}

void loop() 
{
  lcd.setCursor(0, 0); 
  lcd.print(String(counter, 2)); // will be left distance; 2 decimal places

  lcd.setCursor(6, 0); 
  lcd.print(String(counter, 2)); // will be center distance; 2 decimal places

  lcd.setCursor(12, 0); 
  lcd.print(String(counter, 2)); // will be right distance; 2 decimal places
  
  lcd.setCursor(0, 1);  // will be car mode
  if( counter < 2.0 )
    lcd.print("INIT"); 
  else if (counter < 4.0 )
    lcd.print("STOP");
  else if (counter < 6.0 )
    lcd.print("GO  ");
  else if (counter < 8.0 )
    lcd.print("CFG ");
  else
    lcd.print("SET ");

  lcd.setCursor(6, 1); 
  lcd.print(String(counter, 2)); // will be rear distance; 2 decimal places

  lcd.setCursor(12, 1); 
  lcd.print(String(counter-5.0, 2)); // will be steering; 2 decimal places


  counter += 0.01;
  if( counter >= 10.0 )
    counter = 0.0;  
}
