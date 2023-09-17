#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ESP32-BLE-Combo/BleCombo.h"
#include "typedef.h"
#include "messaging/MessageContainer.h"
#include "messaging/MessageParser.h"
#include "main.h"

void setup() {
  Keyboard.begin();
  Mouse.begin();

  pinMode(ignitionSwitchPin, INPUT);
  pinMode(ignitionSwitchLedPin, OUTPUT);
  pinMode(starterButtonPin, INPUT);
  pinMode(starterButtonLedPin, OUTPUT);
  pinMode(runningLedPin, OUTPUT);

  pinMode(ledActivatePin, OUTPUT);
  pinMode(minusPin, INPUT);
  pinMode(plusPin, INPUT);
  pinMode(keyPin, INPUT);

  Serial.begin(115200);
  Serial.println("Boot complete");
}


void loop() {
  loopStart();
  
  readSerialMessage();

  starterButtonBox();

  loopEnd();
}

void loopStart() {
  Keyboard.press((byte) 0x0);
}

void loopEnd() {
  delay(globalDelay);
  globalTicks++;
  Keyboard.release((byte) 0x0);
  Serial.println();
}

bool readSerialMessage() {
  String input = MessageParser::readStringFromSerial();
  
  if (input.length() > 0) {
    lastMessage = MessageParser::parseString(input);
    return true;
  } else {
    return false;
  }
}

void starterButtonBox() {
  ignition();
  starter();
  engineRunning();
  readEngineMode();
}

void ignition() {
  int switchSignal = digitalRead(ignitionSwitchPin);

  Serial.print("i:");
  Serial.print(switchSignal);

  if (!lastIgnitionState && switchSignal == HIGH) {
    digitalWrite(ignitionSwitchLedPin, HIGH);
    lastIgnitionState = true;
    Keyboard.print("i");
  } 
  if (lastIgnitionState && switchSignal == LOW) {
    digitalWrite(ignitionSwitchLedPin, LOW);
    lastIgnitionState = false;
    Keyboard.print("i");
  }
}

void starter() {
  isStarterPressed = digitalRead(starterButtonPin);

  Serial.print(", s:");
  Serial.print(isStarterPressed);

  if (isStarterPressed == HIGH) {
    digitalWrite(starterButtonLedPin, HIGH);
    Keyboard.press(0x73);
  } else {
    digitalWrite(starterButtonLedPin, LOW);
    Keyboard.release(0x73);
  }
}

void readEngineMode() {
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

bool isIgnitionOnByTelemetry() {
  float value = 0.0;
  lastMessage.getValueByMessageType("IO", &value);

  return value == 1.0;
}

bool isEngineOnByTelemetry() {
  float value = 0.0;
  lastMessage.getValueByMessageType("EO", &value);

  return value == 1.0;
}

void engineRunning() { // 0 = ignition off, 1 = ignition on, engine off, 2 = starting, 3 = started
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
    }
}

void engineRunningSwitchState() {
  if (engineRunningLedState) {
    digitalWrite(runningLedPin, LOW);
    engineRunningLedState = false;
  } else {
    digitalWrite(runningLedPin, HIGH);
    engineRunningLedState = true;
  }
}

void everyNTicks(void (*func)(), int ticks) {
  if (globalTicks % ticks == 0) {
    func();
  }
}