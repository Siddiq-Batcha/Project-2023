#include <WiFi.h>

const int buzzerPin = 23; // Define the pin for the buzzer
bool isConnected = false; // Variable to track Wi-Fi connection status

#define led 2
void setup() {
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  pinMode(led,OUTPUT);

  // Connect to Wi-Fi network
  WiFi.begin("ESP", "12345678");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  isConnected = true; // Set connection status to true once connected
  Serial.println("Connected to WiFi");
}

void loop() {
  // Check if the ESP32 is connected to Wi-Fi
  if (WiFi.status() == WL_CONNECTED  && isConnected == true) {
    digitalWrite(led,HIGH);
    isConnected = true; // Set connection status to true
    soundBuzzer();
    Serial.println("hi");
    
  } else if (WiFi.status() != WL_CONNECTED) {
    isConnected = false; // Set connection status to false if not connected
   digitalWrite(led,LOW);
  }
}

void soundBuzzer() {
  // Sound the buzzer three times
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);
  }
}