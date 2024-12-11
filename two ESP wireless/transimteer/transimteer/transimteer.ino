 ///// webserver with ajax
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
// #include <ESP8266WebServer.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>




#define Buzzer  5





WebServer server(80);

const char* ssid = "ESP";  // Enter SSID here
const char* password = "12345678";  //Enter Password here


/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);



  


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
  
  
 
  server.begin();
  Serial.println("HTTP server started");
  
  pinMode(Buzzer, OUTPUT);
  
}

void loop()
{
  server.handleClient();
  
 Serial.println("Server ON");
  
}
