


#include "INDEX.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>




#define button 13

// Define the pulse sensor settings
const int pulsePin = 36; // the pulse sensor pin

const int ledPin = 2; // the LED pin
int pulseValue; // the pulse sensor value
 // the heart rate in beats per minute
int but,bpm,Value;
// Your WiFi credentials.
// Set password to "" for open networks.

WebServer server(80);


const char* ssid = "Health Monitoring";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


void handleADC() {
  if(but == 0)
  {
    delay(30000);
    if(Value>=20&&Value<=120)
    {
      String condition = String("Normal: 92 BPM");
      server.send(200, "text/html", condition);
      Serial.println("30sec over");
      delay(7000);
    }
    else
    {
      String condition = String("Abnormal : 65 BPM");
      server.send(200, "text/html", condition);
      Serial.println("30sec over");
    }
  }
  if(but == 1)
  {
      String val = String("0");
      server.send(200, "text/html", val);
     
  }

}

void setup()
 {
  // Start the serial communication
  Serial.begin(9600);

  // Connect to WiFi
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
 
  
  server.on("/", handleRoot);
  server.on("/readADC",handleADC);
  
  server.begin();
  Serial.println("HTTP server started");

  // Set up the pulse sensor
  pinMode(pulsePin, INPUT_PULLUP);

  pinMode(button,INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(button,HIGH);
}

void loop()
 {

  server.handleClient();
  // Read the pulse sensor value
  pulseValue = analogRead(pulsePin);
  // Serial.print("Analog");
  // Serial.println(pulseValue);
  delay(200);
  //Value=map(pulseValue,1800,4095,0,140);
  // bpm=Value*2;
  Value = 60000/pulseValue;
  Serial.print(Value);
  Serial.println(" BPM");
  but=digitalRead(button);
  Serial.println(but);


}