#include <Wire.h>

#define AHT21B_ADDRESS 0x38 // I2C address of the AHT21B sensor
#define SAMPLE_RATE 1.0 // Sample rate in seconds

float temperatureValues[100]; // Array to store temperature values
int temperatureIndex = 0; // Index to keep track of the current temperature value

void setup() {
  Serial.begin(9600); // Initialize serial communication
  Wire.begin(); // Initialize the I2C bus
}

void loop() {
  float temperature = readTemperature(); // Read temperature from the sensor
  Serial.println(temperature);
  temperatureValues[temperatureIndex] = temperature; // Store temperature in the array
  temperatureIndex = (temperatureIndex + 1) % 60; // Increment temperature index (with wrap-around)
  

  if (temperatureIndex == 0) {
    float respiratoryRate = calculateRespiratoryRate(temperatureValues, SAMPLE_RATE); // Calculate respiratory rate
    Serial.print("Respiratory Rate: ");
    Serial.print(respiratoryRate);
    Serial.println(" breaths per minute");
  }

  delay(1000); // Delay for 1 second
}

float readTemperature() {
  Wire.beginTransmission(AHT21B_ADDRESS);
  Wire.write(0xAC); // Command to trigger temperature measurement
  Wire.write(0x33); // Checksum value
  Wire.endTransmission();

  delay(80); // Wait for temperature measurement to complete

  Wire.beginTransmission(AHT21B_ADDRESS);
  Wire.write(0xA8); // Command to read temperature
  Wire.write(0x00); // Data length = 0
  Wire.endTransmission(false);

  Wire.requestFrom(AHT21B_ADDRESS, 3);
  if (Wire.available() == 3) {
    uint16_t rawTemperature = Wire.read() << 8;
    rawTemperature |= Wire.read();

    float temperature = ((rawTemperature >> 2) * 165.0 / 16383.0) - 40.0;
    return temperature;
  }

  return -1.0; // Return error value if temperature reading fails
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

  float respiratoryRate = (breathCount / 100.0) / sampleRate * 60.0; // Calculate respiratory rate
  return respiratoryRate;
}
