#include "EngineStarter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ESP32-BLE-Combo/BleCombo.h"
#include <HardwareSerial.h>

EngineStarter::EngineStarter(
    int ignitionSwitchPin,
    int starterButtonPin,
    MessageContainer* lastMessage,
    bool lastIgnitionState,
    bool engineRunningLedState,
    bool isStarterPressed
) : ignitionSwitchPin(ignitionSwitchPin), lastMessage(lastMessage),
    starterButtonPin(starterButtonPin), lastIgnitionState(lastIgnitionState),
    engineRunningLedState(engineRunningLedState), isStarterPressed(isStarterPressed) {
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
    lastIgnitionState = true;
    Keyboard.print("i");
  } 
  if (lastIgnitionState && switchSignal == LOW) {
    lastIgnitionState = false;
    Keyboard.print("i");
  }
}

void EngineStarter::starter() {
  isStarterPressed = digitalRead(starterButtonPin);

  Serial.print(", s:");
  Serial.print(isStarterPressed);

  if (isStarterPressed == HIGH) {
    Keyboard.press(0x73);
  } else {
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
  lastMessage->getValueByMessageType("IO", &value);

  return value == 1.0;
}

bool EngineStarter::isEngineOnByTelemetry() {
  float value = 0.0;
  lastMessage->getValueByMessageType("EO", &value);

  return value == 1.0;
}