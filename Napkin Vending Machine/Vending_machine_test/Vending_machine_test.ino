int Quantity = 0, input = 0;
char size;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Im in");
  
  
    
  
  
}

void loop() {
  if(Serial.available())
  {
    size = Serial.read();
    Serial.print("Enter S for Small \n M for Medium \n L for Large: ");
    Serial.println(size); 
    if(size == 's'|| size == 'S')
    {
      Serial.println("im in");
      if (Serial.available())
      {
        Quantity = Serial.parseInt();
        Serial.print("Enter Number of Qunatity: ");
        Serial.println(Quantity);  
  
        for(int i =1; i<Quantity; i++)
        {
          Serial.print("Number:");
          Serial.println(i);
        }
      }
    } 
    else if(size == 'm' || size == 'M')
    {
      Quantity = Serial.parseInt();
      Serial.print("Enter Number of Qunatity: ");
      Serial.println(Quantity);
      
      for(int i =1; i<Quantity; i++)
      {
        Serial.print("Number:");
        Serial.println(i);
      }
    }
    else if(size == 'l' || size == 'L') 
    {
      Quantity = Serial.parseInt();
      Serial.print("Enter Number of Qunatity: ");
      Serial.println(Quantity);
      
      for(int i =1; i<Quantity; i++)
      {
        Serial.print("Number:");
        Serial.println(i);
      }
    }
  //}
  }    
}
