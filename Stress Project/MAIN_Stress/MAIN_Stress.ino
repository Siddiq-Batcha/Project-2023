#include <MPU9250_asukiaaa.h>
#define RXp2 16


float temp;
int tempPin = 36;



#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif


MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float pitch;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2);
  while(!Serial);
  Serial.println("started");
  

#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  // You can set your own offset for mag values
  // mySensor.magXOffset = -50;
  // mySensor.magYOffset = -55;
  // mySensor.magZOffset = -10;
}

void loop() {


   temp = analogRead(tempPin);
   // read analog volt from sensor and save to variable temp
   temp = temp * 0.48828125;
   // convert the analog volt to its temperature equivalent
   Serial.print("TEMPERATURE = ");
   Serial.print(temp); // display temperature value
   Serial.print("*C");
   Serial.println();
   delay(1000);
   
  
  Serial.println("Blood pressure: ");
  Serial.println(Serial2.readString());


  
  uint8_t sensorId;
  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    //Serial.println("accelX: " + String(aX));
    //Serial.println("accelY: " + String(aY));
    //Serial.println("accelZ: " + String(aZ));
    //Serial.println("accelSqrt: " + String(aSqrt));

     //pitch = atan2 (accelY ,( sqrt ((accelX * accelX) + (accelZ * accelZ))));
     pitch = atan2 (aY ,( sqrt ((aX * aX) + (aZ * aZ))));
     pitch = pitch*57.3;

     Serial.println(" Angle: " + String(pitch));
     
  } else {
    Serial.println("Cannod read accel values");
  }

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    //Serial.println("gyroX: " + String(gX));
    //Serial.println("gyroY: " + String(gY));
    //Serial.println("gyroZ: " + String(gZ));
  } else {
    Serial.println("Cannot read gyro values");
  }

  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
    if(mDirection<0)
    {
      mDirection=360+mDirection;
    }
    //Serial.println("magX: " + String(mX));
    //Serial.println("maxY: " + String(mY));
    //Serial.println("magZ: " + String(mZ));
    Serial.println("horizontal direction: " + String(mDirection));
  } else {
    //Serial.println("Cannot read mag values");
  }

  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line
  delay(500);
}
