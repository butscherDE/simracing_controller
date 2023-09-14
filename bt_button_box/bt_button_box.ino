#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <BleCombo.h>
#include "typedef.h"
#include "MessageContainer.h"
#include "MessageParser.h"

#define MAX_STRING_LENGTH 50
#define MAX_ARRAY_SIZE 10

const int globalDelay = 10;
int globalTicks = 0;

MessageContainer lastMessage = MessageParser::parseString("IO0;EO0;");

// Starter Box related
const int ignitionSwitchPin = 18;
const int ignitionSwitchLedPin = 19;
const int starterButtonPin = 22;
const int starterButtonLedPin = 23;
const int runningLedPin = 5;

// Turn Button related
const int ledActivatePin = 32;
const int minusPin = 33;
const int plusPin = 34;
const int keyPin = 35;

bool lastIgnitionState;
bool engineRunningLedState;
bool isStarterPressed;

int engineMode = 0; // 0 = ignition off, 1 = ignition on, engine off, 2 = starting, 3 = started

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
  turnButton();

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

void turnButton() {
  minus();
  plus();
  key();
  keyLed();
}

void minus() {
  int minusSignal = digitalRead(minusPin);

  //Serial.print("minusSignal ");
  //Serial.println(minusSignal);

  if (minusSignal == HIGH) {
    Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
  } 
}

void plus() {
  int plusSignal = digitalRead(plusPin);

  if (plusSignal == HIGH) {
    Keyboard.write(KEY_MEDIA_VOLUME_UP);
  }
}

void key() {
  int keySignal = digitalRead(keyPin);

  //Serial.print("keySignal ");
  //Serial.println(keySignal);

  if (keySignal == HIGH) {
    Keyboard.write(KEY_MEDIA_MUTE);
  }
}

void keyLed() {
  int keySignal = digitalRead(keyPin);

  if (keySignal == HIGH) {
    digitalWrite(ledActivatePin, HIGH);
  } else {
    digitalWrite(ledActivatePin,  LOW);
  }
}