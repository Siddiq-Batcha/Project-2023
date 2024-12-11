#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
//----------------------------------------

#include "Index.h" //--> Our HTML webpage contents with javascripts

#define ON_Board_LED 2  //--> Defining an On Board LED (GPIO2 = D4), used for indicators when the process of connecting to a wifi router and when there is a client request.

//----------------------------------------Create a name and password for the access point on NodeMCU ESP12E
const char* ssid = "NodeMCU_ESP8266";  //--> Access point name
const char* password = "goodluck"; //--> Access point password
//----------------------------------------

WebServer server(80);  //--> Server on port 80

unsigned long previousMillisGetHR = 0; //--> will store the last time Millis (to get Heartbeat) was updated.
unsigned long previousMillisHR = 0; //--> will store the last time Millis (to get BPM) was updated.

const long intervalGetHR = 100; //--> Interval for reading heart rate (Heartbeat) = 10ms.
const long intervalHR = 60000; //--> Interval for obtaining the BPM value based on the sample is 10 seconds.

const int PulseSensorHRWire = 36; //--> PulseSensor connected to ANALOG PIN 0 (A0 / ADC 0).
//const int LED_D1 = D1; //--> LED to detect when the heart is beating. The LED is connected to PIN D1 (GPIO5) on the NodeMCU ESP12E.
int Threshold =519; //--> Determine which Signal to "count as a beat" and which to ignore.

int cntHB = 0; //--> Variable for counting the number of heartbeats.
boolean ThresholdStat = true; //--> Variable for triggers in calculating heartbeats.
int BPMval = 0; //--> Variable to hold the result of heartbeats calculation.

//--------------------------------------------------------------------------------void handleRoot()
// This subroutine is executed when NodeMCU ESP12E IP is opened in the browser.
void handleRoot() {
 String s = MAIN_page; //--> Read HTML contents
 server.send(200, "text/html", s); //--> Send web page
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------void handleHeartRate()
// This subroutine is executed when there is a request from the browser / client.
// This subroutine will send / display the BPM value to the browser.
void handleHeartRate() {
  digitalWrite(ON_Board_LED, LOW); //--> "ON_Board_LED" will be turned on when the request from the browser / client starts.
  String BPMvalSend = String(BPMval); 
  server.send(200, "text/plane", BPMvalSend); //--> Sends BPM value to client request / browser.
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the LED because the client request / browser has been processed.
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------void GetHeartRate()
// This subroutine is for reading the heart rate and calculating it to get the BPM value.
// To get a BPM value based on a heart rate reading for 10 seconds.
void GetHeartRate() {
  //----------------------------------------Process of reading heart rate.
  unsigned long currentMillisGetHR = millis();

  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;

    int PulseSensorHRVal = analogRead(PulseSensorHRWire);

    if (PulseSensorHRVal > Threshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
      //digitalWrite(LED_D1,HIGH);
    }

    if (PulseSensorHRVal < Threshold) {
      ThresholdStat = true;
      //digitalWrite(LED_D1,LOW);
    }
  }
  //----------------------------------------

  //----------------------------------------The process for getting the BPM value.
  unsigned long currentMillisHR = millis();

  if (currentMillisHR - previousMillisHR >= intervalHR) {
    previousMillisHR = currentMillisHR;

    BPMval = cntHB * 6; //--> The taken heart rate is for 10 seconds. So to get the BPM value, the total heart rate in 10 seconds x 6.
    Serial.print("BPM : ");
    Serial.println(BPMval);
    
    cntHB = 0;
  }
  //----------------------------------------
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------void setup()
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //pinMode(LED_D1,OUTPUT); //--> Set LED_3 PIN as Output.

  WiFi.softAP(ssid, password);  //--> Start Making ESP8266 NodeMCU as an access point
  IPAddress apip = WiFi.softAPIP(); //--> Get the IP server
  Serial.print("Connect your wifi laptop/mobile phone to this NodeMCU Access Point : ");
  Serial.println(ssid);
  Serial.print("Visit this IP : ");
  Serial.print(apip); //--> Prints the IP address of the server to be visited
  Serial.println(" in your browser.");
  Serial.println();
  //----------------------------------------

  server.on("/", handleRoot); //--> Which routine to handle at root location. This is display page
  server.on("/getHeartRate", handleHeartRate); //--> This page is called in the javascript code in the Index.h file

  server.begin(); //--> Start server
  Serial.println("HTTP server started");

  Serial.println();
  Serial.println("Please wait 10 seconds to get the BPM Value");
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------void loop()
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
//  WiFiClient client = server.available(); //--> Handle client requests
  GetHeartRate(); //--> Calling the GetHeartRate() subroutine
}
