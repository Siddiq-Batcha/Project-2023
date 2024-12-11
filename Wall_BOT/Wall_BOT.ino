#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> // header files


#include "HX711.h"


/* Put your SSID & Password */
const char* ssid = "Wall robo";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

HX711 scale;

float oldWeight = 0.0;
float newWeight = 0.0;

uint8_t Relay4 = 14;
uint8_t Relay3 = 13;

uint8_t Relay2 = 12;
uint8_t Relay1 = 2;



bool Button1status = LOW;
bool Button2status = LOW;
bool Button3status = LOW;
bool Button4status = LOW;


// Servo servo;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);   //

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
  
  server.on("/Button4on", handle_Button4on);
  server.on("/Button4off", handle_Button4off);


    
  server.begin();
  Serial.println("HTTP server started");


  scale.begin(4, 5); // DT pin = 4, SCK pin = 5
  //scale.set_scale(2280.f); // set the calibration factor (change this value to match your load cell)
  //scale.set_scale(125.5); // set the calibration factor (change this value to match your load cell)
  scale.set_scale(125); // set the calibration factor (change this value to match your load cell)

  scale.tare();
  delay(1000);
  Serial.println("Keep Weight ");

  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, HIGH);
  
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay2, HIGH);
  
  pinMode(Relay3, OUTPUT);
  digitalWrite(Relay3, HIGH);
  
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay4, HIGH);

 
  delay(2000);

}



void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  delay(2000);
  float weight = scale.get_units();



  if(weight < 15 ) {
    oldWeight = weight;
  }

    newWeight = weight - oldWeight;
  
    Serial.print("Weight: ");
    Serial.print(newWeight);
    Serial.println(" g");



 

  if( Button1status || (newWeight >100 && newWeight < 199))
  {
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 On");
          
  }
  else 
  {
      digitalWrite(Relay1, HIGH);
      Serial.println("Relay 1 Off");
  }
  
  if ( Button2status || (newWeight >200 && newWeight < 299) )
  {
     digitalWrite(Relay2, LOW);
     Serial.println("Relay 2 On");
  }
  else 
  {
    digitalWrite(Relay2, HIGH);
    Serial.println("Relay 2 Off");
  }
  

  if( Button3status || (newWeight >300 && newWeight < 399)  )
  {
      digitalWrite(Relay3, LOW);
      Serial.println("Relay 3 On");
  }
  else
  {
      digitalWrite(Relay3, HIGH);
      Serial.println("Relay 3 Off");
  }
  if( Button4status || (newWeight >400 && newWeight < 499)  )
  {
      digitalWrite(Relay4, LOW);
      Serial.println("Relay 3 On");
  }
  else
  {
      digitalWrite(Relay4, HIGH);
      Serial.println("Relay 3 Off");
  }

  

}

void handle_OnConnect() {
  
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;

  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status )); 
}

void handle_Button1on() {
  Button1status = HIGH;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status ));  
}

void handle_Button1off() {
  
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status ));  
  
}

void handle_Button2on() {
  Button1status = HIGH;
  Button2status = HIGH;
  Button3status = LOW;
  Button4status = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status));  
}

void handle_Button2off() {
  
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status));  
  
}

void handle_Button3on() {
  Button1status = HIGH;
  Button2status = HIGH;
  Button3status = HIGH;
  Button4status = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status));  
}

void handle_Button3off() {
  
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status ));  
  
}


void handle_Button4on() {
  
  Button1status = HIGH;
  Button2status = HIGH;
  Button3status = HIGH;
  Button4status = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status));  
}

void handle_Button4off() 
{
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status));  
  
}


String SendHTML(uint8_t Button1stat,uint8_t Button2stat, uint8_t Button3,uint8_t Button4){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #85AF38;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1> WEB PAGE </h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(Button1stat)
  {ptr +="<p>Button1: ON</p><a class=\"button button-off\" href=\"/Button1off\">OFF</a>\n";}
  else
  {ptr +="<p>Button1: OFF</p><a class=\"button button-on\" href=\"/Button1on\">ON</a>\n";}

  if(Button2stat)
  {ptr +="<p>Button2: ON</p><a class=\"button button-off\" href=\"/Button2off\">OFF</a>\n";}
  else
  {ptr +="<p>Button2: OFF</p><a class=\"button button-on\" href=\"/Button2on\">ON</a>\n";}

   if(Button3)
  {ptr +="<p>Button3: ON</p><a class=\"button button-off\" href=\"/Button3off\">OFF</a>\n";}
  else
  {ptr +="<p>Button3: OFF</p><a class=\"button button-on\" href=\"/Button3on\">ON</a>\n";}

   if(Button4)
  {ptr +="<p>Button4: ON</p><a class=\"button button-off\" href=\"/Button4off\">OFF</a>\n";}
  else
  {ptr +="<p>Button4: OFF</p><a class=\"button button-on\" href=\"/Button4on\">ON</a>\n";}

  


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
