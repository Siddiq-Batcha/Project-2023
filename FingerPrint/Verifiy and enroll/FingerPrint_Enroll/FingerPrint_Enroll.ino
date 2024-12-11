#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#define Enroll_pin  8
#define relay1      9
FPS_GT511C3 fps(4, 5);          //    digital pin 5(arduino Tx, fps Rx)
 int EnrollRead;
                                //    digital pin 4(arduino Rx, fps Tx)
void setup()
{
  Serial.begin(9600);           //default baud rate
  delay(100);
  fps.Open();
  fps.SetLED(false);
 pinMode(Enroll_pin,INPUT);
 digitalWrite(Enroll_pin,LOW);
 pinMode(relay1,OUTPUT);
 digitalWrite(relay1,HIGH);
  Enroll();
}
 
 
void Enroll()
{
  int enrollid = 0;             // find open enroll id
  bool okid = true;
  while (okid == true)
  {
    okid = fps.CheckEnrolled(enrollid);
    if (okid==true) enrollid++;
  }
  
  if(EnrollRead == 1)
  {
     
    fps.EnrollStart(enrollid);   // enroll
 
  fps.SetLED(true);
  delay(1000);
  fps.SetLED(false);
  delay(1000);
  fps.SetLED(true);
  delay(1000);
  fps.SetLED(false);
  Serial.print("Press finger to Enroll #"); 
  fps.SetLED(true);
  Serial.println(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    Serial.println("Remove finger");
    fps.SetLED(false);
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    fps.SetLED(true);
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      Serial.println("Remove finger");
      fps.SetLED(false);
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      fps.SetLED(true);
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        Serial.println("Remove finger");
        fps.SetLED(false);
        iret = fps.Enroll3();
        if (iret == 0)
        {
          fps.SetLED(true);
          delay(1000);
          fps.SetLED(false);
          delay(500);
          fps.SetLED(true);
          delay(500);      
          fps.SetLED(false);
          Serial.println("Enrolling Successfull");
        }
        else
        {
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
        }
      }
      else Serial.println("Failed to capture third finger");
    }
    else Serial.println("Failed to capture second finger");
  }
  else Serial.println("Failed to capture first finger");
  }
  
}
 
 
void loop()
{
  EnrollRead = digitalRead(Enroll_pin);
  Enroll();
  Serial.println(EnrollRead);
  if (fps.IsPressFinger() || fps.SetLED(true) )
  {
    

    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
      Serial.print("Verified ID:");
      Serial.println(id);
      digitalWrite(relay1,LOW);
      Serial.println("Relay on");
    }
    else
    {
      Serial.println("Finger not found");
      fps.SetLED(false);
    }
  }
  else
  {
    //Serial.println("Please press finger");
   // digitalWrite(relay1,HIGH);
  }
  delay(100);
}