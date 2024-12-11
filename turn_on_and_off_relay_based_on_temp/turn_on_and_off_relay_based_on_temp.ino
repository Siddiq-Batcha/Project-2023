#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#include <dht.h>  // Include library
#define outPin           10  // Defines pin number to which the sensor is connected
#define relay1           11  //For Heat
#define relay2           12  //For cool
#define VoltageSensor    A0 //VOLTAGE SENSOR


#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
dht DHT;      // Creates a DHT object
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float referenceVoltage = 5.0;
const float maxVoltage = 12.0; 
float VoltageValue,voltage;
void setup() {
 Serial.begin(9600);
  pinMode(VoltageSensor,INPUT);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  // digitalWrite(relay1,LOW);
  // digitalWrite(relay2,LOW);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

}

void loop() {
  
 
  int readData = DHT.read11(outPin);

  float t = DHT.temperature;  // Read temperature
  float h = DHT.humidity;  
 
  VoltageValue = analogRead(VoltageSensor);
  voltage      = VoltageValue * (referenceVoltage/1023);
float percentage = (voltage / maxVoltage) * 100;   // Convert voltage to percentage  
  Serial.print("Batter Voltage = ");
  Serial.println(voltage);
  oled.clearDisplay();    
  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display
  oled.print("Temp="); // text to display
  oled.println(t);
  oled.print("Bat%="); // text to display
  oled.print(percentage);
  oled.display();               // show on OLED // Read humidity

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("°C | ");
  //Serial.print((t*9.0)/5.0+32.0); // Convert celsius to fahrenheit
  Serial.println("°F ");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println("% ");
  Serial.println("");
  Serial.print(percentage);
  Serial.println("%");
  delay (2000);// wait two seconds;



  //fot heat
  // if(t < 36 )
  // { 
  //   pinMode(relay1,OUTPUT);
  //   pinMode(relay2,INPUT);

  //   //  digitalWrite(relay1,LOW);
  //   Serial.println("Relay 1 ON ");
  // }
 

   //....for cool
  if(t > 36)
  {
    pinMode(relay2,OUTPUT);
    pinMode(relay1,INPUT);
    // digitalWrite(relay2,LOW);
    Serial.println("Relay 2 ON ");
  }
   else 
  {
    pinMode(relay1,INPUT);
    pinMode(relay2,INPUT);    
    //  digitalWrite(relay1,HIGH);
    //  digitalWrite(relay2,HIGH);
    Serial.println("Relay 1 OFF");
    Serial.println("Relay 2 OFF");
  }

  // else if(t == 36 )
  // {
  //   pinMode(relay2,INPUT);    
  //   // digitalWrite(relay2,LOW);
     
  //   Serial.println("Relay 2 OFF");
  // }
}
