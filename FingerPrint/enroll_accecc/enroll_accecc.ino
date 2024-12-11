
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"


/* Put your SSID & Password */
const char* ssid = "Wall robo";  // Enter SSID here
const char* password = "12345678";  //Enter Password here


/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t Relay1 = 2;
bool Button1status = LOW;
 
FPS_GT511C3 fps(4, 5);          //    digital pin 5(arduino Tx, fps Rx)
                                //    digital pin 4(arduino Rx, fps Tx)
void setup()
{
  Serial.begin(9600);           //default baud rate

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
  digitalWrite(Relay1, HIGH);
  delay(100);
  fps.Open();
  fps.SetLED(true);
 
  Enroll();
}
 
 
void Enroll()
{
  int enrollid = 0;             // find open enroll id
  bool okid = true;
  while (okid == true)
  {
    okid = fps.CheckEnrolled(enrollid);
    if (okid==true) enrollid++;
  }
  if( Button1status )
  {
    fps.EnrollStart(enrollid);   // enroll      
  
  
  Serial.print("Press finger to Enroll #"); 
  Serial.println(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    Serial.println("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      Serial.println("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        Serial.println("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0)
        {
          Serial.println("Enrolling Successfull");
        }
        else
        {
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
        }
      }
      else Serial.println("Failed to capture third finger");
    }
    else Serial.println("Failed to capture second finger");
  }
    else Serial.println("Failed to capture first finger");
  }
}
 
 
void loop()
{
  server.handleClient();
  delay(100);

  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
      Serial.print("Verified ID:");
      Serial.println(id);
      digitalWrite(Relay1,LOW);
    }
    else
    {
      Serial.println("Finger not found");
      digitalWrite(Relay1,HIGH);
    }
  }
  else
  {
    Serial.println("Please press finger");
  }
  delay(100);
}




void handle_OnConnect() {
  
  Button1status = LOW;

  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status)); 
}

void handle_Button1on() {
  Button1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Button1status));  
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
  ptr +="<h1> Finger Print Enroll </h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(Button1stat)
  {ptr +="<p>Enroll: ON</p><a class=\"button button-off\" href=\"/Button1off\">OFF</a>\n";}
  else
  {ptr +="<p>Enroll: OFF</p><a class=\"button button-on\" href=\"/Button1on\">ON</a>\n";}
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
