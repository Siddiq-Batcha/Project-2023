
#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "index.h"

#define Lout1      32
#define Lout0      33
#define OutputECG  36
#define Buzz       25


MAX30105 particleSensor;

WebServer server (80);

const char* ssid = "Health Monitoring";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);



const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
long irValue;
long delta;
String Bpm,BPMVAL;
float ReadOutput;

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleADC()
{

  BPMVAL = Bpm;
  server.send(200, "text/html", BPMVAL);
}
void handleADC1() {
  if(beatsPerMinute < 40 )
  {
    String value = String("LOW BP ");
    server.send(200, "text/html", value);
  }
  else
  {
    String value = String("Normal ");
    server.send(200, "text/html", value);
  }
  
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);

  server.on("/", handleRoot);
  server.on("/readADC",handleADC);
  server.on("/readADC1",handleADC1);
  server.on("/readECG",handleECG);
  
  pinMode(Lout1,INPUT);
  pinMode(Lout0,INPUT);
  pinMode(OutputECG,OUTPUT);
  pinMode(Buzz,OUTPUT);


  server.begin();
  Serial.println("HTTP server started");

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
  irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);
    Bpm =String(beatsPerMinute);
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

  if(beatsPerMinute < 40 )
  {
    digitalWrite(Buzz,HIGH);
  }
  else
  {
    digitalWrite(Buzz,LOW);
  }
  

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
}


void handleECG()
{
  if(ReadOutput == 4095)
  {
   String ECG = String(" Low ECG ");
    server.send(200, "text/html", ECG);
  }
  else
  {
    String ECG = String(" Normal ");
    server.send(200, "text/html", ECG);
  }
}


