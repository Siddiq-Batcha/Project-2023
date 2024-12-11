#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>


/* Put your SSID & Password */
const char* ssid = "BOREWELL";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint8_t Relay3 = 18;    //light
uint8_t Relay2 = 19;   //electro
uint8_t Relay1 = 17;   //pump


#define trigger      22
#define echo         23
#define gassensor    25


bool Button1status = LOW;
bool Button2status = LOW;
bool Button3status = LOW;

String distString = "";
String gasString = "";





void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/Button1on", handle_Button1on);
  server.on("/Button1off", handle_Button1off);
  
  server.on("/Button2on", handle_Button2on);
  server.on("/Button2off", handle_Button2off);
  
  server.on("/Button3on", handle_Button3on);
  server.on("/Button3off", handle_Button3off);
  
  
    
  server.begin();
  Serial.println("HTTP server started");


  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  pinMode(gassensor,INPUT);
  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, HIGH);
  
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay2, HIGH);
 pinMode(Relay3, OUTPUT);
 digitalWrite(Relay3, HIGH);

  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

   digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
float duration = pulseIn(echo, HIGH); 
float distance = (duration / 2) / 29.1;
distString = String(distance);
Serial.println(distance);
delay(1000);
if(distance > 60 && distance < 90)
{
  distString = String("detection");
}
else
{
  distString = String("0");
}
  int readgas = digitalRead(gassensor);
  //gasString = String(readgas);
  if(readgas == 0)
  {
    gasString = String("Smoke detected");    
  }
  else
  {
    gasString = String("0");
  }
  
  Serial.println(readgas);
  
  if( Button1status)
  {
    pinMode(Relay1, OUTPUT);
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 On");
          
  }
  else 
  {
    pinMode(Relay1, INPUT);
    digitalWrite(Relay1, HIGH);
    Serial.println("Relay 1 Off");
  }
  
  if ( Button2status  )
  {
    digitalWrite(Relay2, LOW);
    Serial.println("Relay 2 On");
  }
  else 
  {
    digitalWrite(Relay2, HIGH);
    Serial.println("Relay 2 Off");
  }
  

  if( Button3status)
  {
    digitalWrite(Relay3, LOW);
    Serial.println("Relay 3 On");
  }
  else
  {
    digitalWrite(Relay3, HIGH);
    Serial.println("Relay 3 Off");
  }
}  
void handle_OnConnect() {
  
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;


  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString)); 
}

void handle_Button1on() {
  Button1status = HIGH;
  
  
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString ));  
}

void handle_Button1off() {
  
  Button1status = LOW;

  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString));  
  
}

void handle_Button2on() {
  
  Button2status = HIGH;
  
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString ));  
}

void handle_Button2off() {
  
 
  Button2status = LOW;
 
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString ));  
  
}

void handle_Button3on() {
 
  Button3status = HIGH;
 
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString));  
}

void handle_Button3off() {
  
  
  Button3status = LOW;
  
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status,distString,gasString ));  
  
}







String SendHTML(uint8_t Button1stat,uint8_t Button2stat, uint8_t Button3, String distS, String gasS){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>TITLE</title>\n";
  ptr +="<style>html { font-family: Electra; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #AC1820;}\n";
  ptr +=".button-on:active {background-color: #CFD5FF;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="<script>function autoRefresh() {  window.location = window.location.href;} setInterval('autoRefresh()', 2000); </script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1> IOT  </h1>\n";
  ptr +="<h2> IOT  </h1>\n";
  ptr +="<h3>Distance "+ distS +" </h3>\n";
  ptr +="<h3>Gas "+ gasS +" </h3>\n";
  
   if(Button1stat)
  {ptr +="<p>Unit1: ON</p><a class=\"button button-off\" href=\"/Button1off\">OFF</a>\n";}
  else
  {ptr +="<p>Unit1: OFF</p><a class=\"button button-on\" href=\"/Button1on\">ON</a>\n";}

  if(Button2stat)
  {ptr +="<p>Unit2: ON</p><a class=\"button button-off\" href=\"/Button2off\">OFF</a>\n";}
  else
  {ptr +="<p>Unit2: OFF</p><a class=\"button button-on\" href=\"/Button2on\">ON</a>\n";}

   if(Button3)
  {ptr +="<p>Unit3: ON</p><a class=\"button button-off\" href=\"/Button3off\">OFF</a>\n";}
  else
  {ptr +="<p>Unit3: OFF</p><a class=\"button button-on\" href=\"/Button3on\">ON</a>\n";}



  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
