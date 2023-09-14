#include <Arduino.h>
#include <esp32-hal.h>
//#include <pins_arduino.h>
//#include "esp32/Arduino.h"
#include "FanTacho.h"

static volatile int counter_rpm = 0;
int last_rpm = 0;
unsigned long millisecondsLastTachoMeasurement = 0;

void IRAM_ATTR FanTacho::rpmFan() {
  counter_rpm++;
}

FanTacho::FanTacho(int value) {
  tachoPin = value;
}

void FanTacho::initTacho(void) {
  pinMode(tachoPin, INPUT);
  digitalWrite(tachoPin, HIGH);
  Serial.print("tachoPin: ");
  Serial.println(tachoPin);
  attachInterrupt(digitalPinToInterrupt(tachoPin), rpmFan, FALLING);
}

void FanTacho::updateTacho(void) {
  // start of tacho measurement
  if ((unsigned long)(millis() - millisecondsLastTachoMeasurement) >= tachoUpdateCycle)
  { 
    // detach interrupt while calculating rpm
    detachInterrupt(digitalPinToInterrupt(tachoPin)); 
    // calculate rpm
    last_rpm = counter_rpm * ((float)60 / (float)numberOfInterrupsInOneSingleRotation) * ((float)1000 / (float)tachoUpdateCycle);
    Serial.print("fan rpm = ");
    Serial.println(last_rpm);

    // reset counter
    counter_rpm = 0; 
    // store milliseconds when tacho was measured the last time
    millisecondsLastTachoMeasurement = millis();

    // attach interrupt again
    attachInterrupt(digitalPinToInterrupt(tachoPin), rpmFan, FALLING);
  }
}