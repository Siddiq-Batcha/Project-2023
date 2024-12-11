#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30102_PulseOximeter.h"

//TwoWire Wire(0);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temperatureValues[100]; // Array to store temperature values
int temperatureIndex = 0; // Index to keep track of the current temperature value

#define REPORTING_PERIOD_MS     1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation 
PulseOximeter pox;
uint32_t tsLastReport = 0;

#define SAMPLE_RATE 1.0 // Sample rate in seconds


// Callback (registered below) fired when a pulse is detected 
void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing..");
  delay(3000);

  if (!mlx.begin()) { // Initialize the MLX90614 sensor
    Serial.println("Temperature Sensor Failed");
    for (;;) {}
  } else {
    Serial.println("Temperature Sensor SUCCESS");
  }

  // Initialize the PulseOximeter instance
  if (!pox.begin()) {
    Serial.println("PuleOximeter FAILED");
    for (;;) {}
  } else {
    Serial.println("PuleOximeter SUCCESS");
  }

  // The default current for the IR LED is 50mA and is changed below
  pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'H') {
      Serial.println("Enter Heart");
      // Make sure to call update as fast as possible
      for(int i =0; i<60; i++)
      {
        pox.update();
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
        {
          Serial.print("Heart rate: ");
          Serial.print(pox.getHeartRate());
          Serial.print(" bpm / SpO2: ");
          Serial.print(pox.getSpO2());
          Serial.print("%");
          Serial.println();

          tsLastReport = millis();
        }

      }
      

     
    } else if (command == 'T') {
      Serial.println("Enter Temp");
      float temperature = mlx.readObjectTempC();
      float fren = mlx.readObjectTempF();

      temperatureValues[temperatureIndex] = temperature;
      temperatureIndex = (temperatureIndex + 1) % 60;
      

      if (temperatureIndex == 0) {
        Serial.println();
        Serial.print("Temp: ");
        Serial.println(temperature);
        float respiratoryRate = calculateRespiratoryRate(temperatureValues, SAMPLE_RATE);
        Serial.print("Respiratory Rate: ");
        Serial.print(respiratoryRate);
        Serial.println(" breaths per minute");
      }

      delay(1000);
    }
  }
}

float calculateRespiratoryRate(float temperatureValues[], float sampleRate) {
  float threshold = 0.12;
  int breathCount = 0;
  float previousTemp = temperatureValues[0];

  for (int i = 1; i < 60; i++) {
    float temperature = temperatureValues[i];
    float difference = temperature - previousTemp;
    if (difference > threshold) {
      breathCount++;
    }
    previousTemp = temperature;
  }

  float respiratoryRate = (breathCount / 60.0) / sampleRate * 60.0;
  return respiratoryRate;
}
