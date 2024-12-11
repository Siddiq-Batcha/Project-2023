
#include <WiFi.h>  
#include <WiFiClient.h>
#include <WebServer.h>

#include "HX711.h"

HX711 scale;
float oldWeight = 0.0;
float totalweight;
int IGNI = 18;
int RASHPIN = 14;

bool igniFlag = LOW;
int MOTORPIN = 22;


WebServer server (80);
/* Put your SSID & Password */
const char* ssid = "OVERLOAD MONITORING SYSTEM";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

bool  engineStatus = LOW;
String warningMessage = "System Ready";

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);

  scale.begin(4, 5); // SDT pin = 4, SCK pin = 5
  //scale.set_scale(2280.f); // set the calibration factor (change this value to match your load cell)
  scale.set_scale(125.1); // set the calibration factor (change this value to match your load cell)
  scale.tare();
  delay(1000);
  Serial.println("Keep Weight ");
  
  digitalWrite(IGNI,LOW);
  digitalWrite(RASHPIN,LOW);  
  pinMode(MOTORPIN,INPUT);  

  server.on("/", handle_OnConnect);
  
  server.begin();
  Serial.println("HTTP server started");

  delay(1000);

}

void loop() {

  // put your main code here, to run repeatedly:
  server.handleClient();

  //scale.set_scale(125.1); // set the calibration factor (change this value to match your load cell)
  //scale.tare();
  //Serial.println("Keep Weight ");
  delay(100);
  float weight = scale.get_units();

  int a = digitalRead(IGNI);
  Serial.println(a);

  int b = digitalRead(RASHPIN);
  Serial.println(b);

  if(a==1){
    igniFlag = HIGH;
    warningMessage =  "Engine Started";
    handle_Engineon();    
  }

  if(b==0){
      Serial.println("Engine Stop ");
      pinMode(MOTORPIN,INPUT);
      igniFlag = LOW;
      warningMessage =  "Engine Stopped : Rash Driving";
      handle_Engineoff();
  }

  if(igniFlag){
    Serial.println("Motor Started");
    pinMode(MOTORPIN,OUTPUT);  
  }

  float newWeight = 0.0;

  if(weight < 15 ) {
    oldWeight = weight;
  }

    newWeight = weight - oldWeight;
    totalweight = newWeight/2;
    Serial.print("Weight: ");
    Serial.print(totalweight);
    Serial.println(" g");

    if(totalweight > 100 && totalweight <200){
      Serial.println("Warning ");
      digitalWrite(IGNI,HIGH);
      warningMessage =  "Warning : Heavy Weight";
      handle_Enginewarning();
    }

    if(totalweight > 200){
      Serial.println("Engine Stop ");
      pinMode(MOTORPIN,INPUT);
      igniFlag = LOW;
      float moreWeight = totalweight - 200;
      String moreWeightStr = String(moreWeight);
      warningMessage =  "Engine Stopped : Weight Overload : Remove "+moreWeightStr+" g";
      handle_Engineoff();
    }
}

void handle_OnConnect() {
  engineStatus = LOW;
  Serial.println("Engine Status OFF");
  server.send(200, "text/html", SendHTML(engineStatus,warningMessage)); 
}

void handle_Engineon() {
  engineStatus = HIGH;
  Serial.println("Engine Status ON");
  server.send(200, "text/html", SendHTML(engineStatus,warningMessage)); 
}

void handle_Enginewarning() {
  engineStatus = HIGH;
  Serial.println("Engine Status Warning");
  server.send(200, "text/html", SendHTML(engineStatus,warningMessage)); 
}

void handle_Engineoff() {
  engineStatus = LOW;
  Serial.println("Engine Status OFF");
  server.send(200, "text/html", SendHTML(engineStatus,warningMessage)); 
}

String SendHTML(uint8_t EngineStat,String warnMessage){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Weight Engine System</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="<script>function autoRefresh() {  window.location = window.location.href;} setInterval('autoRefresh()', 2000); </script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1> VEHICLE MONITORING SYSTEM </h1>\n";

  ptr +="<h3> "+ warnMessage+" </h1>\n";
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
