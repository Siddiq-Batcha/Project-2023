#include<SoftwareSerial.h>
SoftwareSerial myserial (3,2);
void setup() {
  // put your setup code here, to run once:
  myserial.begin(9600);
}

void loop() {
  Serial.println("I'm in");
}
