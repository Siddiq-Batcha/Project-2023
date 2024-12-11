#include <ESP8266WiFi.h>
// #include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Servo.h>

Servo servo;

/* Put your SSID & Password */
const char* ssid = "Auto Clean";  // Enter SSID here
const char* password = "12345678";  //Enter Password

IPAddress local_ip(192,168,43,1);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

#define Relay_1        5               // for Forward    d1
#define Relay_2        4              // for Backward   d2
#define input_swt      12              //d6                               Servo D1
#define Relay_3        14             //D5 water motor

int input;


bool Relay1_sts = LOW;
bool Relay2_sts = LOW;
bool Start = LOW;
bool flag = LOW;
int i = 0 , a;
void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  delay(100);

  server.on("/", handle_OnConnect);
  
  server.on("/Forward_on", handle_relay1_on);
  server.on("/Forward_off", handle_relay1_off);
  
  server.on("/Backward_on", handle_relay2_on);
  server.on("/Backward_off", handle_relay2_off);

  server.on("/Start_on", handle_start_on);
  server.on("/Start_off", handle_start_off);

  server.begin();

  Serial.println("HTTP server started");

  pinMode(Relay_1,OUTPUT);
  pinMode(Relay_2,OUTPUT);
  pinMode(input_swt,INPUT);

  digitalWrite(Relay_1,HIGH);
  digitalWrite(Relay_2,HIGH);
  digitalWrite(Relay_3,HIGH);
  servo.attach(0);
  servo.write(0);
}
void loop()
{
  server.handleClient();
  

  input = digitalRead(input_swt);
  Serial.println(input);

  if(Relay1_sts)
  {
    digitalWrite(Relay_1,LOW);
    pinMode(Relay_1,OUTPUT);
    Serial.println("Forward on");
  }
  else
  {
    digitalWrite(Relay_1,HIGH);
    pinMode(Relay_1,INPUT);
    // Serial.println("Forward off");
  }
  if(Relay2_sts)
  {
    digitalWrite(Relay_2,LOW);
    pinMode(Relay_2,OUTPUT);
    Serial.println("Backward on");
    // if(input == LOW )
    // {
    //   digitalWrite(Relay_2,HIGH);
    //   Serial.println("Backward off");
    // }
  }
  else
  {
    digitalWrite(Relay_2,HIGH);
    pinMode(Relay_2,INPUT);
    // Serial.println("Backward off");
  }
  if(Start)
  { 
    digitalWrite(Relay_3,LOW);// water motor ONN
    // digitalWrite(Relay_2,LOW);// piston inlet
    // pinMode(Relay_2,OUTPUT);
    pinMode(Relay_3,OUTPUT);
    // Serial.println("Backward on");
    // delay(5000);
    
    // if(input == LOW && flag == LOW)// check the inlet is completed
    // {
      //  digitalWrite(Relay_2,HIGH); // piston off
      // pinMode(Relay_2,INPUT);
      // Serial.println("Backward off");
      
      for ( i = 0 ; i< 5; i++)// serov for cleaning the HP sen
      {
        servo.write(180);
        delay(1000);
        servo.write(0);
        delay(1000);
        Serial.print("Servo = ");
        Serial.println(i);
      } 
      digitalWrite(Relay_3,HIGH);
      pinMode(Relay_3,INPUT);
      // a = 1;
    //   digitalWrite(Relay_3,HIGH);// water motor OFF
    //   digitalWrite(Relay_1,LOW); // piston downlet
    //   pinMode(Relay_1,OUTPUT);
    //   pinMode(Relay_3,INPUT);
    //   delay(2000);
    //   Serial.println("Forward on");
    //   a = 1;
    //   Serial.println("Flag HIGH");
    //   flag = HIGH;
 
    // }
    // if(a == 1)
    // {
    //   Serial.print("Start  LOW");
    //   
    //   // flag = LOW;
    //   //handle_start_off();
    //   Start = LOW;// exit from the loop
      
    // }
    
  }
  // else
  // {
  //   digitalWrite(Relay_2,HIGH);
  //   flag = LOW;
  //   // Serial.println("Backward off");
  // }
  // Serial.println(flag);
  // Serial.println("Exit");

}



void handle_OnConnect() {
  
  Relay1_sts = LOW;
  Relay2_sts = LOW;
  Start      = LOW;

  
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start )); 
}

void handle_relay1_on() {
  Relay1_sts = HIGH;
  Relay2_sts = LOW;
  Start      = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start));  
}

void handle_relay1_off() {
  
  Relay1_sts = LOW;
  Relay2_sts = LOW;
  Start      = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start ));  
  
}

void handle_relay2_on() {
  Relay1_sts = LOW;
  Relay2_sts = HIGH;  // Corrected this line to set HIGH
  Start      = LOW;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start));  
}
void handle_relay2_off() {
  
  Relay1_sts = LOW;
  Relay2_sts = LOW;
  Start      = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start ));  
  
}

void handle_start_on() {
  Relay1_sts = LOW;
  Relay2_sts = LOW;
  Start      = HIGH;
  
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start));  
}

void handle_start_off() {
  
  Relay1_sts = LOW;
  Relay2_sts = LOW;
  Start      = LOW;
  
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(Relay1_sts, Relay2_sts, Start));  
  
}





String SendHTML(uint8_t Leftstat,uint8_t Rightstat, uint8_t Straight){
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
  ptr +="<h1> Auto Cleaning System For PH Sensor </h1>\n";
  
  
   if(Leftstat)
  {ptr +="<p>Forward: ON</p><a class=\"button button-off\" href=\"/Forward_off\">OFF</a>\n";}
  else
  {ptr +="<p>Forward: OFF</p><a class=\"button button-on\" href=\"/Forward_on\">ON</a>\n";}

  if(Rightstat)
  {ptr +="<p>Backward: ON</p><a class=\"button button-off\" href=\"/Backward_off\">OFF</a>\n";}
  else
  {ptr +="<p>Backward: OFF</p><a class=\"button button-on\" href=\"/Backward_on\">ON</a>\n";}

   if(Straight)
  {ptr +="<p>Clean: ON</p><a class=\"button button-off\" href=\"/Start_off\">OFF</a>\n";}
  else
   {ptr +="<p>Clean: OFF</p><a class=\"button button-on\" href=\"/Start_on\">ON</a>\n";}


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}