#include "ESP32-BLE-Combo/BleCombo.h"
#include "typedef.h"

#define MAX_STRING_LENGTH 50
#define MAX_ARRAY_SIZE 10

const int globalDelay = 1000;
int globalTicks = 0;

const int ignitionSwitchPin = 18;
const int ignitionSwitchLedPin = 19;
const int starterButtonPin = 22;
const int starterButtonLedPin = 23;
const int runningLedPin = 5;

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
  Serial.begin(115200);
  Serial.println("Boot complete");
}


void loop() {
  Keyboard.press((byte) 0x0);

  ignition();
  starter();
  engineRunning();
  readEngineMode();

  char** protocoll = readSerialProtocol();
  //MyTuple* messages = parseAll(protocoll);
  
  /*
  for (int i = 0; i < 3; ++i) {
    MyTuple message = messages[i];
    Serial.print(message.messageType);
    Serial.print(": ");
    Serial.println(message.numericValue);
  }*/

  delay(globalDelay);
  globalTicks++;
  Keyboard.release((byte) 0x0);
}

void starter() {
  isStarterPressed = digitalRead(starterButtonPin);

  Serial.print("Starter: ");
  Serial.println(isStarterPressed);

  if (isStarterPressed == HIGH) {
    digitalWrite(starterButtonLedPin, HIGH);
    Keyboard.press(0x73);
  } else {
    digitalWrite(starterButtonLedPin, LOW);
    Keyboard.release(0x73);
  }
}

void ignition() {
  int switchSignal = digitalRead(ignitionSwitchPin);

  Serial.print("Ignition: ");
  Serial.println(switchSignal);

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

void readEngineMode() {
  bool isIgnition = false;
  bool isStarted = false;
  bool isStarterPressed = false;


  if (!isIgnition) {
    Serial.println("Engine mode 0");
    engineMode = 0;
  } else if (isIgnition && !isStarted && !isStarterPressed) {
    Serial.println("Engine mode 1");
    engineMode = 1;
  } else if (isIgnition && isStarted) {
    Serial.println("Engine mode 2");
    engineMode = 2;
  } else if (isIgnition && !isStarted && isStarterPressed) {
    Serial.println("Engine mode 3");
    engineMode = 3;
  }
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
            Serial.print("Invalid engine mode: ");
            Serial.println(engineMode)
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

char** readSerialProtocol() {
  if (Serial.available()) {
    char inputString[MAX_STRING_LENGTH];
    int stringIndex = 0;
    
    // Read the string from Serial until newline character is received or max length reached
    while (stringIndex < MAX_STRING_LENGTH - 1) {
      char incomingChar = Serial.read();
      if (incomingChar == '\n') {
        inputString[stringIndex] = '\0'; // Null-terminate the string
        break;
      } else {
        inputString[stringIndex] = incomingChar;
        stringIndex++;
      }
    }

    // Split the string into an array of strings using ';' as the delimiter
    char* arrayStrings[MAX_ARRAY_SIZE];
    char* token = strtok(inputString, ";");
    int arrayIndex = 0;
    
    while (token != NULL && arrayIndex < MAX_ARRAY_SIZE) {
      arrayStrings[arrayIndex] = token;
      arrayIndex++;
      token = strtok(NULL, ";");
    }

/*
    // Print each element of the array
    Serial.println("Split Strings:");
    for (int i = 0; i < arrayIndex; i++) {
      Serial.print("Element ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(arrayStrings[i]);
    }
    */

    return arrayStrings;
  } else {
    return 0;
  }
}

MyTuple* parseAll(char** dataArray) {
    MyTuple* resultArray = new MyTuple[3]; // Example: Assuming we have 3 MyTuple objects in dataArray

    // Iterate through dataArray and parse each element into MyTuple
    for (int i = 0; i < 3; ++i) {
        resultArray[i] = parseMessage(dataArray[i]);
    }

    return resultArray;
}

MyTuple parseMessage(const char* inputString) {
    MyTuple tuple;
    // Extract the message type (first 2 characters)
    strncpy(tuple.messageType, inputString, 2);
    tuple.messageType[2] = '\0'; // Null-terminate the message type buffer

    // Extract the number (remaining characters) and convert it to a double
    tuple.numericValue = atof(inputString + 2);

    return tuple;
}
