void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()==0)
  {

  }
int mynumber=Serial.parseInt();
Serial.println("mynumber");

Serial.println(mynumber);

}
