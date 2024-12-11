#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define SAMPLE_RATE 1.0 // Sample rate in seconds

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temperatureValues[100]; // Array to store temperature values
int temperatureIndex = 0; // Index to keep track of the current temperature value

void setup() {
  Serial.begin(9600); // Initialize serial communication

  mlx.begin(); // Initialize the MLX90614 sensor
}

void loop() {
  float temperature = mlx.readObjectTempC(); 
  float fren = mlx.readObjectTempF();// Read object temperature from the sensor

  temperatureValues[temperatureIndex] = temperature; // Store temperature in the array
  temperatureIndex = (temperatureIndex + 1) % 60; // Increment temperature index (with wrap-around)
  Serial.println(temperature);
  if (temperatureIndex == 0) {
    Serial.println();
    Serial.print("Temp: ");
    Serial.println(temperature);
    float respiratoryRate = calculateRespiratoryRate(temperatureValues, SAMPLE_RATE); // Calculate respiratory rate
    Serial.print("Respiratory Rate: ");
    Serial.print(respiratoryRate);
    Serial.println(" breaths per minute");
  }

  delay(1000); // Delay for 1 second
}

float calculateRespiratoryRate(float temperatureValues[], float sampleRate) {
  float threshold = 0.12; // Threshold to detect breaths, adjust as needed
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

  float respiratoryRate = (breathCount / 60.0) / sampleRate * 60.0; // Calculate respiratory rate
  return respiratoryRate;
}
