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


void loopStart();
bool readSerialMessage();
void starterButtonBox();
void turnButton();
void loopEnd();
void ignition();
void starter();
void engineRunning();
void readEngineMode();
bool isIgnitionOnByTelemetry();
bool isEngineOnByTelemetry();
void everyNTicks(void (*func)(), int ticks);
void engineRunningSwitchState();