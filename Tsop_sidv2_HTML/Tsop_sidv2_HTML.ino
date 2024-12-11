// #include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi.h>  ///// webserver with ajax
#include <WiFiClient.h>
#include <WebServer.h>
#define HEADER                      0x11
#define LOCK_1                      0x66

#define PIN_INN_1             36  // GPIO 5 OR D1
#define PIN_INN_2             35// GPIO 4 OR D2
#define PIN_INN_3             34
#define PIN_INN_4             32
#define PIN_INN_5             33
#define PIN_INN_6             25
#define PIN_INN_7             26
#define PIN_INN_8             27
#define PIN_INN_9             13
#define PIN_INN_10            15
#define PIN_INN_11            16
#define PIN_INN_12            22
#define PIN_INN_13            18
#define PIN_INN_14            19
#define PIN_INN_15            21
#define PIN_INN_16            14
#define PIN_INN_17            17

#define PIN_INN_GND           0


/* Put your SSID & Password */
char* ssid = "DVMS";  // Enter SSID here
char* password = "POEMSIFSDVMS1";  //Enter Password here

const int UDP_PORT = 4444;
const int UDP_SRC_PORT = 4445;
const char *udpAddress = "192.168.0.50";

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 228);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);

String top,bottom,left,right,centre,topmsg,bottommsg,leftmsg,rightmsg,centremsg;
//Are we currently connected?
boolean blnconnected = false;
//The udp library class
WiFiUDP udp;
WebServer server (80);
void printdebughexstring(char* header, uint8_t *databuf, uint8_t len )
{
   char loginfo[10];
   uint8_t idx= 0;
   memset(loginfo, 0, sizeof(loginfo));

   Serial.print(header);
   Serial.print(" : ");
   for(idx=0; idx<len; idx++)
   {
      sprintf(loginfo, "%02X", databuf[idx] );
      Serial.print(loginfo);
      Serial.print(",");
   }
   Serial.println(" ");
}


void WIFI_Connect()
{
    // Loop continuously while WiFi is not connected
  if (WiFi.status() == WL_CONNECTED)
  {
      if( !blnconnected )
      {
          // Connected to WiFi
          Serial.println();
          Serial.print("Connected! IP address: ");
          Serial.println(WiFi.localIP());
      }

      blnconnected = true;
  }
  else 
  {
      blnconnected = false;
  }
  
}


void setup() {
  // put your setup code here, to run once:

   Serial.begin(9600);

   pinMode(PIN_INN_1, INPUT_PULLUP);    //Pull up the pin, so by default the input will be high.
   pinMode(PIN_INN_2, INPUT_PULLUP);    //Pull up the pin, so by default the input will be high.
   pinMode(PIN_INN_3, INPUT_PULLUP);
   pinMode(PIN_INN_4, INPUT_PULLUP);
   pinMode(PIN_INN_5, INPUT_PULLUP);
   pinMode(PIN_INN_6, INPUT_PULLUP);
   pinMode(PIN_INN_7, INPUT_PULLUP);
   pinMode(PIN_INN_8, INPUT_PULLUP);
   pinMode(PIN_INN_9, INPUT_PULLUP);
   pinMode(PIN_INN_10, INPUT_PULLUP);
   pinMode(PIN_INN_11, INPUT_PULLUP);
   pinMode(PIN_INN_12, INPUT_PULLUP);
   pinMode(PIN_INN_13, INPUT_PULLUP);
   pinMode(PIN_INN_14, INPUT_PULLUP);
   pinMode(PIN_INN_15, INPUT_PULLUP);
   pinMode(PIN_INN_16, INPUT_PULLUP);
   pinMode(PIN_INN_17, INPUT_PULLUP);

   pinMode(PIN_INN_GND, INPUT_PULLUP);
   digitalWrite(PIN_INN_GND, 0);

    WiFi.begin(ssid, password);
    if (WiFi.config(local_IP, gateway, subnet)) 
    {
       Serial.println("Static IP Configured");
    }
    else 
    {
       Serial.println("Static IP Configuration Failed");
    }
        
    WiFi.setAutoConnect (true);
    WiFi.setAutoReconnect (true);
    
    // Connecting to WiFi...
    Serial.print("Connecting to ");
    Serial.print(ssid);

    server.on("/",handle_OnConnect);
    server.begin();

}

bool initudp = false;
uint8_t txbuff[50];
int lock = 0;
int lockcount = 0;
char strlog[32];
void loop() {
      // put your main code here, to run repeatedly:
      server.handleClient();
      int inn1 = digitalRead(PIN_INN_1);
      int inn2 = digitalRead(PIN_INN_2);
      int inn3 = digitalRead(PIN_INN_3);
      int inn4 = digitalRead(PIN_INN_4);
      int inn5 = digitalRead(PIN_INN_5);
      //int inn6 = digitalRead(PIN_INN_6);
      //int inn7 = digitalRead(PIN_INN_7);
      //int inn8 = digitalRead(PIN_INN_8);
      //int inn9 = digitalRead(PIN_INN_9);
      //int inn10 = digitalRead(PIN_INN_10);
      //int inn11 = digitalRead(PIN_INN_11);
      //int inn12 = digitalRead(PIN_INN_12);
      //int inn13 = digitalRead(PIN_INN_13);
      //int inn14 = digitalRead(PIN_INN_14);
      //int inn15 = digitalRead(PIN_INN_15);
      //int inn16 = digitalRead(PIN_INN_16);
      //int inn17 = digitalRead(PIN_INN_17);

      //sprintf(strlog, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", inn1, inn2, inn3, inn4, inn5, inn6, inn7, inn8, inn9, inn10, 
        //             inn11, inn12, inn13, inn14, inn15, inn16, inn17);
      //Serial.println(strlog);
      // Serial.println(inn1);
      // Serial.println(inn2);
      // Serial.println(inn3);
      // Serial.println(inn4);
      // Serial.println(inn5);
      
      
    

      lock = 1;
      if( inn1 == 0 )
      {
        Serial.println("Top");
        topmsg = " TOP Detected";
      }
      // else
      // {
      //   topmsg = " 0 ";
      // }

      if( inn2 == 0 )
      {
          Serial.println("Bottom");
          bottommsg = "Bottom Detected";
      }
      else
      {
        bottommsg = " 0 ";
      }

      if( inn3 == 0 )
      {
        Serial.println("Left");
        leftmsg = "Left Detected";
      }
      else
      {
        leftmsg = " 0 ";
      }

      if( inn4 == 0 )
      {
        Serial.println("Right");
        rightmsg = "Right Detected";
      }
      else
      {
        rightmsg = " 0 ";
      }

      if( inn5 == 0 )
      {
        Serial.println("Center");
        centremsg = "Centre Detected";
      }
      else
      {
        centremsg = " 0 ";
      }
      //if( v1 > 1.5 || v2 > 1.5 || v3 > 1.5 || inn4 == 0 || inn5 == 0 ) 
      //if( inn4 == 0 || inn5 == 0 ) 
      //if( inn1 == 0 || inn2 == 0 || inn3 == 0 || inn4 == 0 || inn5 == 0 || inn6 ==0 || inn7 == 0 || inn8 == 0 || inn9 == 0 || inn10 == 0 || inn11 == 0|| inn12 == 0 || inn13 == 0 || inn14 == 0 || inn15 == 0 || inn16 == 0 || inn17 == 0 )
      // if( inn5 == 0 && inn1 == 1 && inn2 == 1 && inn3 == 1 && inn4 == 1 )
      if( inn5 == 0 || inn1 == 0 || inn2 == 0 || inn3 == 0 || inn4 == 0 )
      {
          Serial.print("lock");
          Serial.println(lockcount);
          ++lockcount;
          if( lockcount > 5)
          {
              lock = 0;
              lockcount = 5;
          }
          //pinMode(PIN_INN_GND, INPUT_PULLUP);
          //delay(1000);

      }
      else
      {
          lockcount = 0;
      }
    
    WIFI_Connect();

  
    if( blnconnected )
    { 
        if(!initudp)
        {
            udp.begin(UDP_SRC_PORT);
            Serial.print("Listening on UDP port ");
            Serial.println(UDP_SRC_PORT);
            
            initudp = true;
        }

        memset(txbuff, 0, sizeof(txbuff));

        txbuff[0] = HEADER;
        txbuff[1] = LOCK_1;
        txbuff[2] = lock;
        txbuff[3] = 0;
        
        printdebughexstring("SENT : ", txbuff, 20 );
        
         //Send a packet
        udp.beginPacket(udpAddress,UDP_PORT);
        udp.write(txbuff, 32);
        udp.endPacket();

    }

    delay(100);

    //pinMode(PIN_INN_GND, OUTPUT);
    

}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(topmsg,bottommsg,leftmsg,rightmsg,centremsg)); 
}

String SendHTML(String top,String bottom,String left,String right,String centre){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Digital Meter</title>\n";
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
  ptr +="<h1> TSOP TEST </h1>\n";
  ptr +="<h3> Tsop INFO: "+ top+" </h3>\n";
  ptr +="<h3> Tsop INFO: "+ bottom+" </h3>\n";
  ptr +="<h3> Tsop INFO: "+ left+" </h3>\n";
  ptr +="<h3> Tsop INFO: "+ right+" </h3>\n";
  ptr +="<h3> Tsop INFO: "+ centre+" </h3>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
