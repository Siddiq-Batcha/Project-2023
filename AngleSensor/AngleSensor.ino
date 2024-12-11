#include <Wire.h>
#include <DFRobot_BMX160.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "Webpage.h"


DFRobot_BMX160 bmx160;

ESP8266WebServer server(80);

const char* ssid = "Robot";  // Enter SSID here
const char* password = "12345678";  //Enter Password here


/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);
float pitch;

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


void setup() {
  Serial.begin(9600);
  Wire.begin();
  bmx160.begin();

  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/readADC",handleADC);
  
  server.begin();
  Serial.println("HTTP server started");
  

  
}

const float Pi = 3.14159;

void loop() {
  //bmx160.read();
     server.handleClient();
    sBmx160SensorData_t Omagn, Ogyro, Oaccel;

  /* Get a new sensor event */
  bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

  pitch = atan2(-Oaccel.x,(sqrt((Oaccel.y*Oaccel.y)+(Oaccel.z*Oaccel.z))));
  pitch = pitch * 57.3;

  Serial.print("Pitch");
  Serial.println(pitch);

  delay(2000);
}

void handleADC() {
  
  String value = String(pitch);
  server.send(200, "text/html", value);

}

