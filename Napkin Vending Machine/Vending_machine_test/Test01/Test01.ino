int Quantity ;
char size;
int total = 100,total1;
void setup() {
  Serial.begin(9600);
  Serial.print("total");
  Serial.println(total);
}

void loop() {
  if (Serial.available()) {
    size = Serial.read();
    Serial.print("Enter S for Small\nM for Medium\nL for Large: ");
    Serial.println(size);

    if (size == 's' || size == 'S' || size == 'm' || size == 'M' || size == 'l' || size == 'L') {
      Serial.print("Enter the quantity: ");
      // while (!Serial.available()); // Wait for input
      // {
      Quantity = Serial.parseInt();
      Serial.println(Quantity);
      // delay(1000);
      for (int i = 1; i <= Quantity; i++) {
        Serial.print("Number: ");
        Serial.println(i);
        total1 = total - i;
        Serial.print("Total:");
        Serial.println(total1);
      }
      // }
    }
    else {
      Serial.println("Invalid size. Please try again.");
    }
  }
  delay(1000);
}
