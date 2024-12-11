const int inputPin = 5;    // Replace with the actual input pin number
const int buzzerPin = 18;   // Replace with the actual buzzer pin number
const int delayTime = 10000;  // Delay time in milliseconds (10 seconds)

unsigned long previousInputTime = 0;
unsigned long currentTime;
void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);  // Make sure buzzer is initially off
}

void loop() {
  
  int inputValue = digitalRead(inputPin);
//  Serial.println(inputValue);
//  delay(500);
  if(inputValue == HIGH)
  {
    
   for(int i= 0;i <=10 ; i++)
   {
     Serial.println("Timer Start");
     Serial.println(i);
     delay(500);
    int inputValue1 = digitalRead(inputPin);
     if(inputValue1 == HIGH)
     {
       digitalWrite(buzzerPin,HIGH);
       Serial.print("BUZ");
     }
     else 
     {
       digitalWrite(buzzerPin,LOW);
     }
      delay(1000);
   }
  }
  //  if(inputValue == HIGH)
  //  {
  //     currentTime = millis();
  //     int a = currentTime/1000;
  //     Serial.println("TIME");
  //     Serial.println(a);
  //     if (currentTime - previousInputTime >= delayTime) {
  //     // 10-second interval has passed, process the input
  //       previousInputTime = currentTime;

  //      // Your code to process the input goes here
  //       Serial.println("Input processed");
  //       delay(200);
     
  // }
  // }
  // // Check if any input received during the 10-second interval
  // if (inputValue == HIGH && (currentTime - previousInputTime < delayTime)) {
  //   // Input received within the 10-second interval, turn on the buzzer
  //   digitalWrite(buzzerPin, HIGH);
  //   // Buzzer on duration
  //   delay(500);
  //   digitalWrite(buzzerPin, LOW);
  //    Serial.println("BUZ");
     
  //    delay(200);
  // }
}
