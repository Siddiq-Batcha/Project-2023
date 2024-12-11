#define RXp2 16
//#define TXp2 1
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2);
}
void loop() {
     
    Serial.println("Blood pressure: ");
    
    Serial.println(Serial2.readString());
}
