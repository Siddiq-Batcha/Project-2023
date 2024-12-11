
#include "index.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

  //Web page header file
#include <MPU9250_asukiaaa.h>
#define RXp2 16
float temp;
float pitch;
int tempPin = 36;
String pit;
String bp;
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif
WebServer server(80);

//Enter your SSID and PASSWORD
// const char* ssid = "StressMonitor";
// const char* password = "123456789";
const char* ssid = "Office 4G";
const char* password = "3dotsinnovations";

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s);
 //Send web page
}
 
void handleADC() {
 
 if(Serial2.available() )
 {
  bp = Serial2.readString();
  String adcValue = (bp);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
 //delay(2000);
 }
}
void handletemp() {
 
   temp = analogRead(tempPin);
   // read analog volt from sensor and save to variable temp
 
 String tempt = String(temp);
 
 server.send(200, "text/plane", tempt); //Send ADC value only to client ajax request
  //delay(2000);


 
}




//===============================================================
// Setup
//===============================================================
MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  
  Serial2.begin(9600, SERIAL_8N1, RXp2);
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);

//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
  while(!Serial);
  Serial.println("started");
  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
  
  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
  server.on("/readtet", handletemp);
  server.on("/readgy", handlegyro);
  
  }

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  server.handleClient();
  
  delay(1);

  temp = temp * 0.48828125;
  // convert the analog volt to its temperature equivalent
//  Serial.print("TEMPERATURE = ");
//  Serial.print(temp); // display temperature value
//  Serial.print("*C");
//  Serial.println();

  //Send ADC value only to client ajax request

}

void handlegyro() {

    uint8_t sensorId;
  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
   

     
    pitch = atan2 (aY ,( sqrt ((aX * aX) + (aZ * aZ))));
    pitch = pitch*57.3;

    Serial.println(" Angle: " + String(pitch));
    pit = String(pitch);
    server.send(200, "text/plane", pit);
     // delay(2000);

  }
}
 
