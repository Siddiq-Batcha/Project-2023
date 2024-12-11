int total = 100, QUA;
char size;
int quantity ;


void setup() 
{
  Serial.begin(9600);
  

}

void loop() 
{
    if (Serial.available()) 
    {
      size = Serial.read();
      Serial.print("Enter S for Small\nM for Medium\nL for Large: ");

      switch(size)
      {
        case 's':
          Serial.print("Entered Size is small");
          Serial.print("Enter Number of Quantity: ");
         
  
           quantity = Serial.parseInt();
          // QUA = quantity.toInt();
          Serial.print(quantity);
          //delay(2000);
          for(int i = 1; i<=quantity; i++ )
          {
            Serial.print("I = ");
            Serial.print(i);
          }
          
           //Serial.println(quantity);

          break;

        case 'm':
          Serial.println("Enter Size is Medium ");
          quantity = Serial.parseInt();
          Serial.print("Enter Number of Quantity: ");
        //   Serial.println(quantity);
        //   delay(2000);
        //   for(int i = 0; i<=quantity; i++ )
        //   {
        //     Serial.print("I = ");
        //     Serial.println(i);
        //   }
        //   break;

        // case 'l':
        //   Serial.println("Enter Size is Large");
        //   quantity = Serial.parseInt();
        //   Serial.print("Enter Number of Quantity: ");
        //   Serial.println(quantity);
        //   delay(2000);
        //   for(int i = 0; i<=quantity; i++ )
        //   {
        //     Serial.print("I = ");
        //     Serial.println(i);
        //   }
        //   break;
          


          
          

      }
    }
}
