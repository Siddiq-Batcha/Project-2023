#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"

#define inpt   16
#define out    4
#define relay  17

int inread;

WebServer server (80);
/* Put your SSID & Password */
const char* ssid = "LPG SENSOR";  // Enter SSID here
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
  
 if(inread == 0)
  {
    
    Serial.println("Smoke detected");
    String adcValue = String("Smoke Detected");
   server.send(200,"text/plane",adcValue);
  }
  
  else
  {
   
    String adcValue = String("0");
   server.send(200,"text/plane",adcValue);
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

  digitalWrite(relay,HIGH);
  
  pinMode(inpt,INPUT);
  pinMode(out,OUTPUT);
  pinMode(relay,OUTPUT);
}
void loop()
{
  server.handleClient(); 
   inread = digitalRead(inpt);
  Serial.print("INPUT");
  Serial.println(inread); 
  int a = digitalRead(relay);
  Serial.print("RELAY");
  Serial.println(a);
  delay(500);

 
  delay(200);
  if(inread == 0)
  {
    digitalWrite(out,HIGH);
    pinMode(relay,OUTPUT);
    Serial.println("Smoke deteced");
    
  }
  else
  {
    pinMode(relay,INPUT);
    digitalWrite(out,LOW);
    Serial.println(" No Smoke deteced");
  }
  
}
