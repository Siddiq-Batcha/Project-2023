
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float lattitude,longitude; // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(3,1);//rx,tx
TinyGPSPlus gps;// create gps object
void setup(){
Serial.begin(9600); // connect serial
lcd.begin(16, 2);
gpsSerial.begin(9600); // connect gps sensor
lcd.print("GPS TRACKER");
delay(2000);
lcd.clear();
}
void loop(){
  
  while(1)
  {
   while (gpsSerial.available() > 0)
   { gps.encode(gpsSerial.read()); }

      if (gps.location.isUpdated())
      {
       Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
       Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
       lattitude=gps.location.lat();
       longitude=gps.location.lng();
      break;
      }
  
   
  
}

 Serial.print("LATTITUDE="); Serial.println(lattitude,6);
 Serial.print("LONGITUDE="); Serial.println(longitude,6);
 lcd.print("LAT ");lcd.print(lattitude,6);
 lcd.setCursor(0, 1);
 lcd.print("LONG ");lcd.print(longitude,6);
 delay(1000);
 lcd.clear();

}
