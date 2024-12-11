#include <MAX3010x.h>

/**
 @file Max30102.ino
 @brief Checks the Heart Rate and SpO2
 @detail Checks the Heart Rate and SpO2 using Max30102 sensor

 * Reference arduino code: https://github.com/bobdavis321/Arduino-MAX30100 , https://www.youtube.com/watch?v=mmXJ33Nx4Mc 
 * Refrence aries board: https://vegaprocessors.in/blog/interfacing-max30102-pulse-oximeter-to-thejas-soc/
 * 
 * MAX30100 sensor
 * Device address -0x57
 * Connections:
 * MAX30102     Aries Board
 * 3V3      -   3.3V
 * GND      -   GND
 * SDA      -   SDA0
 * SCL      -   SCL0
 * For connecting to port 1 (SCL 1 and SDA1) of aries board use the default variable TwoWire Wire(1) instead of TwoWire Wire(0);
 * .
 * Note:
 * Use 10K pull-up resistors while connecting SDA and SCL pins to MAX30102 sensor for accurate measurements.
 **/

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#define I2C_SDA         33
#define I2C_SCL         32

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
//TwoWire Wire();

#include "MAX30102_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
#define SAMPLE_RATE             1.0 // Sample rate in seconds
#define MAX_TEMP_RATE           60

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation 
PulseOximeter pox;
uint32_t tsLastReport = 0;
// Callback (registered below) fired when a pulse is detected 
void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup()
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!mlx.begin(MLX90614_I2CADDR, &Wire)) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        //while (1);
      }
      
      Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
  
    Serial.print("Initializing..");
    delay(3000);
#if 1
    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    // The default current for the IR LED is 50mA and is changed below
    pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
#endif
      
}

int spo2elimatecount = 0;
float Hrate;
uint8_t spo2;

float temperatureValues[100];
int temperatureIndex = 0; 
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
   // long irValue = pox.getHeartRate();
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Hrate = pox.getHeartRate();
        spo2 = pox.getSpO2();
        if( spo2 > 0 )
        {
          ++spo2elimatecount;
          if( spo2elimatecount > 20 )
          {
            spo2elimatecount = 20;
            Serial.print("Heart rate:");
            Serial.print(Hrate);
            Serial.print("bpm / SpO2:");
            Serial.print(spo2);
            Serial.print("%");
          }
        }
        else
        {
            spo2elimatecount = 0;
        }
        
        Serial.println();
        tsLastReport = millis();

        mesuretemperature();

        
    }

       
}

void mesuretemperature()
{

    mlx.readAmbientTempC();
    float temperature =mlx.readObjectTempC();
    mlx.readAmbientTempF();
    mlx.readObjectTempF();
    temperatureIndex = (temperatureIndex + 1) % MAX_TEMP_RATE;

    Serial.print("Temperature: "); 
    Serial.print(temperature); 
    Serial.println(" degrees C");
      
     if (temperatureIndex == 0) {
           float respiratoryRate = calculateRespiratoryRate(temperatureValues, SAMPLE_RATE); // Calculate respiratory rate
          //Serial.print("Temperature: "); 
          //Serial.print(temperature); 
          //Serial.println(" degrees C");
          Serial.println(" Measuring Temperature...");
     }
}

float calculateRespiratoryRate(float temperatureValues[], float sampleRate) {
  float threshold = 0.5; // Threshold to detect breaths, adjust as needed
  int breathCount = 0;
  float sumtemperature=0.0;
  float previousTemp = temperatureValues[0];

  for (int i = 1; i < MAX_TEMP_RATE; i++) {
    float temperature = temperatureValues[i];
    sumtemperature = sumtemperature + temperature;
    float difference = temperature - previousTemp;
    if (difference > threshold) {
      breathCount++;
    }
    previousTemp = temperature;
  }

  sumtemperature = sumtemperature / MAX_TEMP_RATE;

   Serial.print("Temperature: "); 
   Serial.print(sumtemperature); 
   Serial.println(" degrees C");
   Serial.println( " ");

  float respiratoryRate = (breathCount / 60.0) / sampleRate * 60.0; // Calculate respiratory rate
  return respiratoryRate;
}
