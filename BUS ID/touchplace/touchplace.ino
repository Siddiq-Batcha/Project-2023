#include "index.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>


#define Touch1 36
#define Touch2 35
#define Touch3 34

int TouchRead1, TouchRead2, TouchRead3;


WebServer server(80);



//Enter your SSID and PASSWORD
const char* ssid = "TOUCH";
const char* password = "123456789";



IPAddress local_ip(192,168,23,10);
IPAddress gateway(192,168,23,10);
IPAddress subnet(255,255,255,0);

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s);
 //Send web page
}

void handleADC() {
 

if(TouchRead1 == 1)
{
  String Message1 = " New Bus stand";
  server.send(200, "text/plane", Message1); //Send ADC value only to client ajax request
}
else
{
  String value1 = "0";
  server.send(200, "text/plane", value1); //Send ADC value only to client ajax request
}

}

void handleADC2() {
 

if(TouchRead2 == 1)
{
  String Message2 = " Old Bus stand";
  server.send(200, "text/plane", Message2); //Send ADC value only to client ajax request
}
else
{
  String value2 = "0";
  server.send(200, "text/plane", value2); //Send ADC value only to client ajax request
}

}
void handleADC3() {
 

if(TouchRead3 == 1)
{
  String Message3 = " Big Temple";
  server.send(200, "text/plane", Message3); //Send ADC value only to client ajax request
}
else
{
  String value3 = "0";
  server.send(200, "text/plane", value3); //Send ADC value only to client ajax request
}

}

void setup()
{
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);


  pinMode(Touch1,INPUT);
  pinMode(Touch2,INPUT);
  pinMode(Touch3,INPUT);


  server.begin();
  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
  server.on("/readADC2", handleADC2);
  server.on("/readADC3", handleADC3);
}
void loop()
{
 server.handleClient();

 TouchRead1 = digitalRead(Touch1);
 //Serial.println(TouchRead1);
 TouchRead2 = digitalRead(Touch2);
 TouchRead3 = digitalRead(Touch3);
}
