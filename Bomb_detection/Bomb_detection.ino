// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>

// #include <ESP8266WebServer.h>
#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "LANDMINE DETECTION";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

// ESP8266WebServer server(80);

WebServer server (80);
#define  trigPinleft   33
#define  echoPinleft   32

#define  trigPinright  27
#define  echoPinright  26


#define  MetalDetector 36

#define  Relay1         18             //Left
#define  Relay2         25            //Right
#define  Relay3         21           //Pump



bool Leftstatus = LOW;
bool Rightstatus = LOW;
bool Straightstatus = LOW;
long durationleft, distanceleft,durationright, distanceright;

int metalval;

void setup() {

Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);

    
  server.on("/", handle_OnConnect);
  
  server.on("/Lefton", handle_Lefton);
  server.on("/Leftoff", handle_Leftoff);
  
  server.on("/Righton", handle_Righton);
  server.on("/Rightoff", handle_Rightoff);
  
  server.on("/Straighton", handle_Straighton);
  server.on("/Straightoff", handle_Straightoff);
  
  server.begin();
  Serial.println("HTTP server started");

pinMode(trigPinleft, OUTPUT);
pinMode(echoPinleft, INPUT);

pinMode(trigPinright, OUTPUT);
pinMode(echoPinright, INPUT);
pinMode(MetalDetector, INPUT);
pinMode(Relay1, OUTPUT);
pinMode(Relay2, OUTPUT);
pinMode(Relay3, OUTPUT);
digitalWrite(Relay3, HIGH);
digitalWrite(Relay1, HIGH);
digitalWrite(Relay2, HIGH);
}

 

void loop() {
server.handleClient();
 
metalval = analogRead(MetalDetector);

digitalWrite(trigPinleft,HIGH);

delayMicroseconds(1000);

digitalWrite(trigPinleft, LOW);


 
durationleft=pulseIn(echoPinleft, HIGH);

distanceleft =(durationleft/2)/29.1;


digitalWrite(trigPinright,HIGH);

delayMicroseconds(1000);

digitalWrite(trigPinright, LOW);
 

durationright=pulseIn(echoPinright, HIGH);

distanceright =(durationright/2)/29.1;
Serial.println(distanceright);


if(Straightstatus)
{
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
 
  Serial.println(" Robo moves Staright ");
}
else
{
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
}


if( Leftstatus || distanceright  >= 10 && distanceright <=15 )
{
  digitalWrite(Relay1, HIGH);
  // pinMode(Relay1,OUTPUT);
  Serial.println("Left ");
}
// else
// {
//   digitalWrite(Relay1, HIGH);  
// }

if ( Rightstatus || distanceleft  >= 10 && distanceleft <=15  )
{
  digitalWrite(Relay2, HIGH);
  Serial.println("Right  On");
}
// else
// {
//   digitalWrite(Relay2, HIGH);
// }
// if(metalval > 5)
// {
//   digitalWrite(Relay1, HIGH);
//   digitalWrite(Relay2, HIGH);
//   digitalWrite(Relay3, LOW);
//   delay(1000);
//   digitalWrite(Relay3, HIGH);
//   Serial.println("PUMP Motor on");
// }

}

void handle_OnConnect() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;

  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus )); 
}

void handle_Lefton() {
  Leftstatus = HIGH;
  Rightstatus = LOW;
  Straightstatus = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus));  
}

void handle_Leftoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus ));  
  
}

void handle_Righton() {
  Leftstatus = LOW;
  Rightstatus = HIGH;
  Straightstatus = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus ));  
}

void handle_Rightoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus));  
  
}

void handle_Straighton() {
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus));  
}

void handle_Straightoff() {
  
  Leftstatus = LOW;
  Rightstatus = LOW;
  Straightstatus = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Leftstatus, Rightstatus, Straightstatus));  
  
}










String SendHTML(uint8_t Leftstat,uint8_t Rightstat, uint8_t Straight){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
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
  ptr +="<h1> ROBO </h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(Leftstat)
  {ptr +="<p>LEFT: ON</p><a class=\"button button-off\" href=\"/Leftoff\">OFF</a>\n";}
  else
  {ptr +="<p>LEFT: OFF</p><a class=\"button button-on\" href=\"/Lefton\">ON</a>\n";}

  if(Rightstat)
  {ptr +="<p>RIGHT: ON</p><a class=\"button button-off\" href=\"/Rightoff\">OFF</a>\n";}
  else
  {ptr +="<p>RIGHT: OFF</p><a class=\"button button-on\" href=\"/Righton\">ON</a>\n";}

   if(Straight)
  {ptr +="<p>STRIGHT: ON</p><a class=\"button button-off\" href=\"/Straightoff\">OFF</a>\n";}
  else
  {ptr +="<p>STRIGHT: OFF</p><a class=\"button button-on\" href=\"/Straighton\">ON</a>\n";}


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

