#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char THING_ID[] = "c9f70e7a-0c30-4166-8336-546de5053339"; // Replace with your thing ID
const char DEVICE_KEY[] = "T4UL2QXISGANH1JOIA7Z"; // Replace with your device key

float temperature;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  ArduinoCloud.begin(THING_ID, DEVICE_KEY);
  ArduinoCloud.monitor();
}

void loop() {
  temperature = analogRead(A0) * 0.48828125;
  Serial.println(temperature);
  ArduinoCloud.update();
  delay(1000);
}
