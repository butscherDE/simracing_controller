#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ESP32-BLE-Combo/BleCombo.h"
#include "messaging/MessageContainer.h"
#include "messaging/MessageParser.h"
#include "main.h"



EngineStarter engineStarter(pinIgnitionSwitch, pinEngineStarterButton, &lastMessage);

void setup() {
  Keyboard.begin();

  Serial.begin(115200);
  Serial.println("Boot complete");
}


void loop() {
  loopStart();
  
  readSerialMessage();

  engineStarter.starterButtonBox();

  loopEnd();
}

void loopStart() {
  Keyboard.begin();
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