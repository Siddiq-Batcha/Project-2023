#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#include "Webpage.h"

ESP8266WebServer server(80);

#define   IR         14
#define   Smoke      12
#define   Alcohol    13
#define   Relay1     5
#define   Relay2     4

int irval,smokeval,alcoholval;

const char* ssid = "Arasu Bus";  // Enter SSID here
const char* password = "12345678";  //Enter Password here


/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);


void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


void handleIR() {
   if(irval == 0)
  {
    String irmessage = String("Driver in Sleepy Condition");
    server.send(200, "text/html", irmessage);
    // digitalWrite(Relay1,LOW);
    // digitalWrite(Relay2,LOW);
  }
  else
  {
      String irmessage = String("Normal Condition");
    server.send(200, "text/html", irmessage);
    // digitalWrite(Relay1,HIGH);
    // digitalWrite(Relay2,HIGH);
  } 
}
void handleSmoke() {
   if(smokeval == 0)
  {
    String Smokemessage = String("Smoke Detected in the Bus");
    server.send(200, "text/html", Smokemessage);
    // digitalWrite(Relay1,LOW);
    
  }
  else
  {
    String Smokemessage = String("Normal Condition");
    server.send(200, "text/html", Smokemessage);
    // digitalWrite(Relay1,HIGH);
  
  } 
}
void handleAlcohol() {
   if(alcoholval == 0)
  {
    String Alcoholmessage = String("Alcohol Detected in the Bus");
    server.send(200, "text/html", Alcoholmessage);
    // digitalWrite(Relay1,LOW);
    
  }
  else
  {
    String Alcoholmessage = String("Normal Condition");
    server.send(200, "text/html", Alcoholmessage);
    // digitalWrite(Relay1,HIGH);
  
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
  server.on("/readIR",handleIR);
  server.on("/readSmoke",handleSmoke);
  server.on("/readAlchol",handleAlcohol);
  
  server.begin();
  Serial.println("HTTP server started");


 pinMode(IR,INPUT);
 pinMode(Smoke,INPUT);
 pinMode(Alcohol,INPUT);
 pinMode(Relay1,OUTPUT);
 pinMode(Relay2,OUTPUT);
 digitalWrite(Relay1,HIGH);
 digitalWrite(Relay2,HIGH);
}



void loop()
{
  server.handleClient();


  irval = digitalRead(IR);
  smokeval = digitalRead(Smoke);
  alcoholval = digitalRead(Alcohol);
  Serial.print("Smoke ");
  Serial.println(alcoholval);

  if(irval == 0)
  {
    digitalWrite(Relay1,LOW);
    digitalWrite(Relay2,LOW);    
  }
  else
  {
    digitalWrite(Relay1,HIGH);
    digitalWrite(Relay2,HIGH);
  }
  if(smokeval == 0 || alcoholval == 0)
  {
    digitalWrite(Relay1,LOW);
  }
  else
  {
    digitalWrite(Relay1,HIGH);
  }
 
}