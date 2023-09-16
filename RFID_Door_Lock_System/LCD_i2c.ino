#include <Wire.h>
#include <LiquidCrystal_I2C.h>          // library for LCD with I2C

LiquidCrystal_I2C lcd(0x3F, 16, 2);     // Set the LCD address (0x27) and size of lcd (16 chars and 2 line display)

void setup()
{
  lcd.init();         // initialize the LCD
  lcd.backlight();    // Turn on the blacklight
}

void loop()
{
  lcd.clear();                      // clear the LCD
  lcd.setCursor (3, 0);             // Set the cursor to 1st line
  lcd.print("Get Started");         // print the message
  lcd.setCursor (0, 1);             // Set the cursor to 2nd line
  lcd.print("with ARDUINO");        // print the message
  delay(2000);
  lcd.clear();                      // clear the LCD
  lcd.setCursor (0, 0);             // Set the cursor
  lcd.print("Experiment #6");       // print the message
  lcd.setCursor (3, 1);             // Set the cursor
  lcd.print("LCD with I2C");        // print the message
  delay(2000);
}

