void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(2,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  int x;
  x=digitalRead(2);
  Serial.println(x);

}
