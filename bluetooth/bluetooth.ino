#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String msg;
void setup()
{
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);  
  lcd.print("HELLO ,WORLD");
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    Serial.write("");
    lcd.clear();
    msg = "";
  }
  while (Serial.available())
  {
    delay(100);
    char c  = Serial.read();

    if (c != "") {
      if (c != '\n') {
        Serial.print(c);
        msg += c;
      }
    }
  }
  if (msg.length() > 0 )
  {
    lcd.clear();
    lcd.print(msg);
    Serial.println("LCD Message: " + msg);
  }
  msg = "";
}
