#include<SPI.h>
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
Adafruit_SSD1306 display(-1);
#define    SoilMositure   A0
#define    Relay           3
int Min=0;
int Max=1023;
void setup() {
Serial.begin(9600);
pinMode(SoilMositure,INPUT);
pinMode(Relay,OUTPUT);
digitalWrite(Relay,LOW);
// initialize with the I2C addr 0x3C
display.begin(SSD1306_SWITCHCAPVCC,0x3C);
}
void loop() {
  float SensorValue = analogRead(A0);
  float per = (SensorValue - Max)/(Min - Max)*100;
  Serial.print("PER = ");
  Serial.println(per);
  delay(1000);
  Serial.print(SensorValue);
  delayMicroseconds(2000);
  display.setTextSize(0.2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Soil Moisture(%):");
  display.println(per);
  //display.println("%");
    display.display();
    delay(300);
  // Clear the buffer.
  display.clearDisplay();
  if(per>=50.0)
  {
    digitalWrite(Relay,HIGH);
  }
  else
  {
    digitalWrite(Relay,LOW);
  }
 }
