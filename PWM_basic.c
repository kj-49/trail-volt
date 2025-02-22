// Simplified 20kHz PWM Generator for Buck Converter
#define PWM_PIN 9     // Use Timer1 (16-bit timer)
#define PWM_FREQ 20000 // 20kHz
#define PRESCALER 1
#define TOP_VALUE (F_CPU / (PRESCALER * PWM_FREQ) - 1)

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  
  // Configure Timer1 for Fast PWM
  TCCR1A = 0;
  TCCR1B = 0;
  
  // Mode 14 (Fast PWM with ICR1 as TOP)
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  
  // Set frequency to 20kHz
  ICR1 = TOP_VALUE; // TOP value for 20kHz
  OCR1A = 0;        // Start with 0% duty cycle
}

void loop() {
  // Example: Set duty cycle (0-799 for 0-100%)
  int potValue = analogRead(A0); // Read pot on A0
  OCR1A = map(potValue, 0, 1023, 0, TOP_VALUE);
  delay(10);
}
