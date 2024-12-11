 ///// webserver with ajax
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
// #include <ESP8266WebServer.h>
#include "index.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>




#define TRIGGER_PIN  12
#define ECHO_PIN  14
#define MAX_DISTANCE 10000


long duration, distance;


WebServer server(80);

const char* ssid = "Brick Detector";  // Enter SSID here
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
  if(distance == 29)
  {
    String value = String("Good");
    server.send(200, "text/html", value);
  }
  else if(distance >=30 && distance < 35)
  {
    String value = String("Bad");
    server.send(200, "text/html", value);
  }
  else
  {
    String value = String("0");
    server.send(200, "text/html", value);
  }
  
 
}

void setup()
{
  Serial.begin(115200);

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
  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  server.handleClient();
  

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.println(distance);
}
