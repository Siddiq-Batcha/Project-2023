/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <MPU9250_asukiaaa.h>


 //Web page header file
#define RXp2 16

float temp;
int tempPin = 36;

String strbp, strtemp, strpitch;


#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif
String SendHTML( );

WebServer server(80);



MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float pitch;
//Enter your SSID and PASSWORD
const char* ssid = "Office 4G";
const char* password = "3dotsinnovations";
//void setup()
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
 
void handleADC() {
  temp = analogRead(tempPin);
   // read analog volt from sensor and save to variable temp
   temp = temp * 0.48828125;
   // convert the analog volt to its temperature equivalent
   
//   

 String adcValue = String(temp);

 strtemp = adcValue;
 
 //server.send(200, "text/plane", SendHTML(temp));
 //Send ADC value only to client ajax request
}
void handlebp()
{
  //Serial2.begin(9600, SERIAL_8N1, RXp2);
  //String BP = Serial2.readString();
  //String bp = String(BP);
  //strbp = bp;
  //server.send(200, "text/html", SendHTML());
  
}

  
//===============================================================
// Setup
//===============================================================

void setup(void){
  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, RXp2);
  while(!Serial);
  Serial.println("started");

  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
  #endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
  
  Serial.println();
  Serial.println("Booting Sketch...");

/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handlebp);      //This is display page
  server.on("/readADC", handleADC);
  server.on("/readbp", handlebp);//To get update of ADC Value only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  server.handleClient();
  delay(1);

  Serial.println("TEMPERATURE = ");
  //Serial.println(temp1);
  
   delay(1000);
   
 if( Serial2.available() )
 {
    Serial.println("Blood pressure: ");
    Serial.println(Serial2.readString());
    strbp = Serial2.readString();
 }
 //bloodpressure=Serial2.readString();


 
  uint8_t sensorId;
  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    //Serial.println("accelX: " + String(aX));
    //Serial.println("accelY: " + String(aY));
    //Serial.println("accelZ: " + String(aZ));
    //Serial.println("accelSqrt: " + String(aSqrt));

     //pitch = atan2 (accelY ,( sqrt ((accelX * accelX) + (accelZ * accelZ))));
     pitch = atan2 (aY ,( sqrt ((aX * aX) + (aZ * aZ))));
     pitch = pitch*57.3;

     Serial.println("PITCH: " + String(pitch));
     strpitch = String(pitch);
     
  } else {
    Serial.println("Cannod read accel values");
  }

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    //Serial.println("gyroX: " + String(gX));
    //Serial.println("gyroY: " + String(gY));
    //Serial.println("gyroZ: " + String(gZ));
  } else {
    Serial.println("Cannot read gyro values");
  }

  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
    if(mDirection<0)
    {
      mDirection=360+mDirection;
    }
    //Serial.println("magX: " + String(mX));
    //Serial.println("maxY: " + String(mY));
    //Serial.println("magZ: " + String(mZ));
    Serial.println("horizontal direction: " + String(mDirection));
  } else {
    //Serial.println("Cannot read mag values");
  }

  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line

  server.send(200, "text/html", SendHTML());
  
  delay(1000);
  
}


   
String SendHTML( )
{
  String ptr = "<!DOCTYPE html>\n";
         ptr += "<html>\n";
         ptr += "<head>\n";
         ptr += "<<meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n";
         ptr += "<<meta charset='utf-8'>\n";
         ptr += "<<meta http-equiv='refresh' content='2'>\n";
         ptr += "<style>\n";
         ptr += ".card{\n";
         ptr +=  "max-width: 400px;\n";
         ptr +="min-height: 250px;\n";
         ptr +="background: #02b875;\n";
         ptr +="padding: 30px;\n";
         ptr +="box-sizing: border-box;\n";
         ptr +="color: #FFF;\n";
         ptr +="margin:20px;\n";
         ptr +="box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);\n";
         ptr +="</style>\n";
         ptr +="<body>\n";
        
        ptr +="<div class=\"card\">\n";
        ptr +="<h4>Stress Monitering</h4><br>\n";
        ptr +="<h1>Temperature Value:<span id=\"ADCValue\">"+ strtemp + "</span></h1><br>\n";
        ptr +="<h2>Blood Pressure Value:<span id=\"bp\">" + strbp + "</span></h2><br>\n";
        ptr +="<h2>PITCH Value:<span id=\"bp\">" + strpitch + "</span></h2><br>\n";
        ptr +="<script>\n";
        
        
        ptr +="</script>\n";
        ptr +="</body>\n";
        ptr +="</head>\n";
        ptr +="</html>\n";

  return ptr;


}
