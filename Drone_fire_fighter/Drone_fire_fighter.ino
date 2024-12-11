#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>



/* Put your SSID & Password */
const char* ssid = "fire fighter Drone";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t Relay1 = 2;



bool Button1status = LOW;



// Servo servo;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/Button1on", handle_Button1on);
  server.on("/Button1off", handle_Button1off);
  
 
    
  server.begin();
  Serial.println("HTTP server started");


  
  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1,LOW);
  

 
  delay(2000);

}



void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();



 

  if( Button1status )
  {
     
      digitalWrite(Relay1, HIGH);
      Serial.println("Relay 1 On");
          
  }
  else 
  {
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 Off");
  }
  
  

  

}

void handle_OnConnect() {
  
  Button1status = LOW;
  

  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status)); 
}

void handle_Button1on() {
  Button1status = HIGH;
 
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status ));  
}

void handle_Button1off() {
  
  Button1status = LOW;
 
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status));  
  
}




String SendHTML(uint8_t Button1stat){
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
  ptr +="<h1> The Drone Fire Fighter </h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(Button1stat)
  {ptr +="<p>Drone1: ON</p><a class=\"button button-off\" href=\"/Button1off\">OFF</a>\n";}
  else
  {ptr +="<p>Drone1: OFF</p><a class=\"button button-on\" href=\"/Button1on\">Through</a>\n";}

 

  


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
