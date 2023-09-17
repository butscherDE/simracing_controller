#include "EngineStarter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ESP32-BLE-Combo/BleCombo.h"
#include <HardwareSerial.h>

EngineStarter::EngineStarter(
    HardwareSerial& serial,
    int ignitionSwitchPin,
    int starterButtonPin,
    bool lastIgnitionState,
    bool engineRunningLedState,
    bool isStarterPressed
) : serial(serial), ignitionSwitchPin(ignitionSwitchPin),
    starterButtonPin(starterButtonPin), lastIgnitionState(lastIgnitionState),
    engineRunningLedState(engineRunningLedState), isStarterPressed(isStarterPressed) {
    // Constructor implementation (if needed)
}

void EngineStarter::starterButtonBox() {
  ignition();
  starter();
  engineRunning();
  readEngineMode();
}

void EngineStarter::ignition() {
  int switchSignal = digitalRead(ignitionSwitchPin);

  Serial.print("i:");
  Serial.print(switchSignal);

  if (!lastIgnitionState && switchSignal == HIGH) {
    //digitalWrite(ignitionSwitchLedPin, HIGH);
    lastIgnitionState = true;
    Keyboard.print("i");
  } 
  if (lastIgnitionState && switchSignal == LOW) {
    //digitalWrite(ignitionSwitchLedPin, LOW);
    lastIgnitionState = false;
    Keyboard.print("i");
  }
}

void EngineStarter::starter() {
  isStarterPressed = digitalRead(starterButtonPin);

  Serial.print(", s:");
  Serial.print(isStarterPressed);

  if (isStarterPressed == HIGH) {
    //digitalWrite(starterButtonLedPin, HIGH);
    Keyboard.press(0x73);
  } else {
    digitalWrite(starterButtonLedPin, LOW);
    Keyboard.release(0x73);
  }
}

void EngineStarter::readEngineMode() {
  bool isIgnition = isIgnitionOnByTelemetry();
  bool isStarted = isEngineOnByTelemetry();


  if (!isIgnition) {
    engineMode = 0;
  } else if (isIgnition && !isStarted && !isStarterPressed) {
    engineMode = 1;
  } else if (isIgnition && !isStarted && isStarterPressed) {
    engineMode = 2;
  } else if (isIgnition && isStarted) {
    engineMode = 3;
  }
}

bool EngineStarter::isIgnitionOnByTelemetry() {
  float value = 0.0;
  lastMessage.getValueByMessageType("IO", &value);

  return value == 1.0;
}

bool EngineStarter::isEngineOnByTelemetry() {
  float value = 0.0;
  lastMessage.getValueByMessageType("EO", &value);

  return value == 1.0;
}

void EngineStarter::engineRunning() { // 0 = ignition off, 1 = ignition on, engine off, 2 = starting, 3 = started
    /*
    switch (engineMode) {
        case 0:
            digitalWrite(runningLedPin, LOW);
            break;
        case 1:
            everyNTicks(engineRunningSwitchState, 100);
            break;
        case 2:
            everyNTicks(engineRunningSwitchState, 10);
            break;
        case 3:
            digitalWrite(runningLedPin, HIGH);
            break;
        default:
            break;
    }*/
}

void EngineStarter::engineRunningSwitchState() {
  if (engineRunningLedState) {
    digitalWrite(runningLedPin, LOW);
    engineRunningLedState = false;
  } else {
    digitalWrite(runningLedPin, HIGH);
    engineRunningLedState = true;
  }
}