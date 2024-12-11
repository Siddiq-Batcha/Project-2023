#include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// Set the I2C address of the LCD module
// Use the command "i2c_scanner" to find the address if unknown
// #define LCD_ADDRESS 0x27

// Set the number of columns and rows of the LCD module
// #define LCD_COLUMNS 16
// #define LCD_ROWS     2


#define GasSensor    5
#define Buzz        14

// Create an instance of the LiquidCrystal_I2C class
// LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

float value;
void setup() {
  // Initialize the Wire library
  Wire.begin();

  // Initialize the LCD module
  // lcd.begin();

  // // Turn on the backlight (optional)
  // lcd.backlight();

  // Print a message on the LCD
  
  Serial.begin(9600);
  pinMode(GasSensor, INPUT);
  pinMode(Buzz, OUTPUT);
  digitalWrite(Buzz,LOW);
}

void loop() {
  value = digitalRead(GasSensor);
  Serial.println(value);
  // lcd.setCursor(0, 0);  // Set the cursor to the beginning of the first line
  // lcd.print("Toxic Value:");
  // lcd.setCursor(12, 0);  // Set the cursor to the beginning of the second line
  // lcd.print(value);
  delay(500); 
  if(value == 0)
  {
    // lcd.setCursor(0, 0);  // Set the cursor to the beginning of the first line
    // lcd.print("Smoke Detected");
    digitalWrite(Buzz,HIGH);
  }
  else
  {
    digitalWrite(Buzz,LOW);
  }
}
