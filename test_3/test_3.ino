#define IR_LED_PIN 1 // Replace with the actual pin you are using to drive the IR LED

void setup() {
  pinMode(IR_LED_PIN, OUTPUT);

  // Configure Timer/Counter 1 (TC1)
  TCCR1 = _BV(CTC1) | _BV(CS10); // Set TC1 to CTC (Clear Timer on Compare) mode, no prescaler
  GTCCR = _BV(PWM1B) | _BV(COM1B0); // Enable PWM on OC1B (Pin 6)
  OCR1C = 105; // Set TOP value for TC1 (Controls the frequency)
  OCR1B = 52; // Set Compare value for TC1 (Controls the duty cycle)
}

void loop() {
  // Your main code goes here (optional)
}
