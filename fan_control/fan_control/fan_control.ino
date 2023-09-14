const int pwmPin = 26;  // Pin 26
const int delayTime = 5000; // 1 second delay

void setup() {
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600); // Initialize serial communication
}

void loop() {

  analogWrite(pwmPin, 0);

  delay(delayTime);
  analogWrite(pwmPin, 120);

  delay(delayTime);
  analogWrite(pwmPin, 255);
  delay(delayTime);
}

void lala() {
    // Increase PWM duty cycle
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    analogWrite(pwmPin, dutyCycle);
    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.println("%");
    delay(delayTime);
  }

  // Decrease PWM duty cycle
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    analogWrite(pwmPin, dutyCycle);
    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.println("%");
    delay(delayTime);
  }
}