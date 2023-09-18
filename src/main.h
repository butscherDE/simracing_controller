#define MAX_STRING_LENGTH 50
#define MAX_ARRAY_SIZE 10

#include "engineControl/EngineStarter.h"

const int globalDelay = 10;
int globalTicks = 0;

MessageContainer lastMessage = MessageParser::parseString("IO0;EO0;");


void loopStart();
void loopEnd();
bool readSerialMessage();

// Pins
int pinSDA = 21;
int pinSCL = 22;
int pinIgnitionSwitch = 12;
int pinEngineStarterButton = 13;