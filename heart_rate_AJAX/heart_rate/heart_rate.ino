/*
  Optical Heart Rate Detection (PBA Algorithm) using the MAX30105 Breakout
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  This is a demo to show the reading of heart rate or beats per minute (BPM) using
  a Penpheral Beat Amplitude (PBA) algorithm.

  It is best to attach the sensor to your finger using a rubber band or other tightening
  device. Humans are generally bad at applying constant pressure to a thing. When you
  press your finger against the sensor it varies enough to cause the blood in your
  finger to flow differently which causes the sensor readings to go wonky.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/
#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>

// #include <ESP8266WebServer.h>

#include <Wire.h>
#include "MAX30105.h"


#include "Webpage.h"

#include "heartRate.h"


#define Lout1  32
#define Lout0  33
#define OutputECG  36


WebServer server(80);

String BPM, BPMVAL,ECG,pressure;
long delta;

const char* ssid = "Health Monitoring";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

float ReadOutput;
/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);
long irValue;

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


int pressVal = 90 ;

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);
  
    server.on("/", handleRoot);
  server.on("/readADC",handleADC);
  server.on("/readADC1",handleADC1);
  server.on("/readADC2",handleECG);
  
  
  
  server.begin();
  Serial.println("HTTP server started");
  pinMode(Lout1,INPUT);
  pinMode(Lout0,INPUT);
  pinMode(OutputECG,OUTPUT);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{

  server.handleClient();

ReadOutput = analogRead(OutputECG);
  
  //Wait for a bit to keep serial data from saturating
 

  irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);
    BPMVAL = String(beatsPerMinute);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
}


void handleADC()
{

  BPM = BPMVAL;
  server.send(200, "text/html", BPM);
}
void handleADC1() {

 if(beatsPerMinute < 40 )
 {
   String LOWBPM = String("LOW BP ");
   server.send(200, "text/html", LOWBPM);
 } 
 else
 {
   String LOWBPM = String(" Normal ");
   server.send(200, "text/html", LOWBPM);
   
 }
}
void handleECG()
{
  if(ReadOutput == 4095)
  {
    ECG = String(" Low ECG ");
    server.send(200, "text/html", ECG);
  }
  else
  {
    ECG = String(" Normal ");
    server.send(200, "text/html", ECG);
  }
}