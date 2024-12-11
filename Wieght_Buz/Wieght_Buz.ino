#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>



#include "HX711.h"

HX711 scale;
float oldWeight = 0.0;
float newWeight = 0.0;
float tripWeight = 0.0;
float finalCalculated = 0.0;
float totalInTank = 0.0;



int BUZPIN = 18;

//ESP8266WebServer server (80);
WebServer server (80);
/* Put your SSID & Password */
const char* ssid = "DIGITAL METER";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

boolean  tripStatus = LOW;
String weightMessage = "System Ready";
String totalInMessage = "0";

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  //Serial.println(WiFi.softAPIP());
  delay(100);

  pinMode(BUZPIN,OUTPUT);
  
  scale.begin(4, 5); // DT pin = 4, SCK pin = 5
  //scale.set_scale(2280.f); // set the calibration factor (change this value to match your load cell)
  scale.set_scale(125.1); // set the calibration factor (change this value to match your load cell)
  //scale.set_scale(-1575);
  scale.tare();
  delay(1000);
  //Serial.println("Keep Weight ");

  server.on("/", handle_OnConnect);

  server.on("/Tripon", handle_Tripon);

  server.begin();
  //Serial.println("HTTP server started");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  delay(1000);
  
 float weight = scale.get_units();

    Serial.print("Real Weight: ");
    Serial.print(weight);
    Serial.println(" g  ");

  if(weight < 15 ) {
    oldWeight = weight;
  }

  
    newWeight = weight - oldWeight;
  
    //Serial.print("Weight: ");
    //Serial.print(newWeight);
    //Serial.println(" g  ");

    float finalVolume = (105.0/300.1)*(newWeight-80.0);

    //Serial.print(" Flow Volume  ");
    //Serial.print(volume  * 3.2);
    //Serial.println(" mL  ");

    finalCalculated = (finalVolume  * 3.2) - tripWeight;
    String calculatedVolume = String(finalCalculated);
        
    Serial.print(" Final Volume  ");
    Serial.print(calculatedVolume);
    Serial.println(" mL  ");

  
    totalInTank = finalCalculated + tripWeight;
    totalInMessage = String(totalInTank);


    if(finalCalculated < 50.0){
      weightMessage = "Warning: Low Fuel " + calculatedVolume +" ml";
      Serial.println("LOW Fuel");
      digitalWrite(BUZPIN,HIGH);
    }
    else{
      weightMessage = " " + calculatedVolume +" ml";
      digitalWrite(BUZPIN,LOW);
    }
}

void handle_OnConnect() {
  tripStatus = LOW;
  server.send(200, "text/html", SendHTML(tripStatus,weightMessage,totalInMessage)); 
}

void handle_Tripon() {
  tripStatus = HIGH;
  tripWeight = finalCalculated;
  weightMessage = "Liters 0" ;
  server.send(200, "text/html", SendHTML(tripStatus,weightMessage,totalInMessage)); 
}

String SendHTML(uint8_t tripStat,String weightMess,String totalInMess){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Digital Meter</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1> DIGITAL METER </h1>\n";

  ptr +="<h3> "+ weightMess+" </h3>\n";

  ptr +="<h3> Total Liters "+ totalInMess +" </h3>\n";

   if(tripStat)
  {ptr +="<p>Back</p><a class=\"button button-off\" href=\"/\">Back</a>\n";}
  else
  {ptr +="<p>Trip</p><a class=\"button button-off\" href=\"/Tripon\">Trip</a>\n";}

 if(!tripStat)
  {ptr +="<p>Refresh</p><a class=\"button button-off\" href=\"/\">Refresh</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

