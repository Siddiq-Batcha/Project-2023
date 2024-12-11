#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
 
FPS_GT511C3 fps(4, 5);          //    digital pin 5(arduino Tx, fps Rx)
                                //    digital pin 4(arduino Rx, fps Tx)
void setup()
{
  Serial.begin(9600);           //default baud rate
  delay(100);
  fps.Open();
  fps.SetLED(true);
}
 
void loop()
{
 
  // Identify fingerprint test
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
      Serial.print("Verified ID:");
      Serial.println(id);
    }
    else
    {
      Serial.println("Finger not found");
    }
  }
  else
  {
    Serial.println("Please press finger");
  }
  delay(100);
}