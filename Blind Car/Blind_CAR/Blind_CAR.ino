#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
bool flag = true;
BluetoothSerial SerialBT;
#define IR1          23
#define IR2          22  
#define IR3          27
#define IR4          26
#define IR_Centre1   35
#define IR_Centre2   32

#define Relay1       18
#define Relay2       19


#define Tigger       33
#define Echo         25

#define Buzzer       17

long duration;
int distance_cm;

int read_IR1,read_IR2,read_IR3,read_IR4,read_IRCEN1,read_IRCEN2;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("BlindCar"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  pinMode(IR4,INPUT);
  pinMode(IR_Centre1,INPUT);
  pinMode(IR_Centre2,INPUT);
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Tigger, OUTPUT);
  pinMode(Echo, INPUT);
  
}

void loop() {

  digitalWrite(Tigger, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond pulse to trigger the sensor
  digitalWrite(Tigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Tigger, LOW);
  
  // Measure the duration of the echo pulse
  duration = pulseIn(Echo, HIGH);
  
  // Calculate the distance in centimeters
  distance_cm = duration * 0.034 / 2;
  
  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

 
  read_IRCEN1 = digitalRead(IR_Centre1);//centreleft
  // Serial.println(read_IRCEN1);
  read_IRCEN2 = digitalRead(IR_Centre2);//centreright
  Serial.print("IRCen: ");
  Serial.print(read_IRCEN1);
  Serial.print("IRCen2: ");
  Serial.println(read_IRCEN2);
  
  read_IR1 = digitalRead(IR1);//L1
  read_IR2 = digitalRead(IR2);//L2
  read_IR3 = digitalRead(IR3);//R1
  read_IR4 = digitalRead(IR4);//R2

  Serial.println(read_IR2);
  if (distance_cm <= 18)
  {
    Serial.println("Ultrs Stop Car");
    digitalWrite(Relay1, HIGH);
    digitalWrite(Relay2, HIGH);
  }
  else if(read_IRCEN1 == 0)
  {
    digitalWrite(Relay1, HIGH);
  }
  else if (read_IRCEN2 == 0) {
  digitalWrite(Relay2, HIGH);
  }
  else 
  {
    Serial.println("Robot Moves Straight");
    digitalWrite(Relay1, LOW);
    digitalWrite(Relay2, LOW);
  }

  if (SerialBT.available()) 
  {
    char Read_BLUE =SerialBT.read();
    Serial.print(Read_BLUE);
   if (Read_BLUE == '1')
    {
    Serial.println("IN IF LOOP");
    while (read_IR1 != 1) 
    {
        Serial.println("Going to S1");
        read_IR1 = digitalRead(IR1);  // Update read_IR1 inside the loop
        if(read_IR1 == 1)
        {
          digitalWrite(Buzzer, HIGH);
          delay(20);
          digitalWrite(Buzzer, LOW);
          delay(20);
          digitalWrite(Buzzer, HIGH);
          delay(20);
          digitalWrite(Buzzer, LOW);
          Serial.println("S1 Stopping");
          delay(2000);
        }
    }
    
    }

    if(Read_BLUE == '2')
    {
    //  while (read_IR1 != 0 && read_IR2 != 0) 
    while (read_IR2 != 1) 
    {
        
        Serial.println("Going to S2");
        
        read_IR1 = digitalRead(IR1); 
        read_IR2 = digitalRead(IR2);  // Update read_IR1 inside the loop
        // if(read_IR1 == 0 && read_IR2 == 0)
        if( read_IR2 == 1)
        {
          digitalWrite(Buzzer, HIGH);
          delay(30);
          digitalWrite(Buzzer, LOW);
          delay(30);
          digitalWrite(Buzzer, HIGH);
          delay(30);
          digitalWrite(Buzzer, LOW);
          Serial.println("S2 Stopping");
          delay(2000);
        }
    } 
    }

     if(Read_BLUE == '3')
    {
     while (read_IR3 != 0) 
    {
        
        Serial.println("Going to S3");
        
        read_IR3 = digitalRead(IR3);  // Update read_IR1 inside the loop
        if(read_IR1 == 0)
        {
          digitalWrite(Buzzer, HIGH);
          delay(50);
          digitalWrite(Buzzer, LOW);
          delay(50);
          digitalWrite(Buzzer, HIGH);
          delay(50);
          digitalWrite(Buzzer, LOW);
          Serial.println("S3 Stopping");
        }
    } 
    }





  }
  delay(50);
   
}

