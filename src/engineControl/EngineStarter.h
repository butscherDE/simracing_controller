#ifndef ENGINESTARTER_H
#define ENGINESTARTER_H

#include "../messaging/MessageContainer.h"

class EngineStarter {

private:
    int ignitionSwitchPin;
    bool lastIgnitionState;
    bool engineRunningLedState;
    bool isStarterPressed;
    int starterButtonPin;
    int engineMode = 0; // 0 = ignition off, 1 = ignition on, engine off, 2 = starting, 3 = started
    MessageContainer* lastMessage; // Pointer to telemetry data struct
public:
    EngineStarter(
        int ignitionSwitchPin,
        int starterButtonPin,
        MessageContainer* lastMessage,
        bool lastIgnitionState = false,
        bool engineRunningLedState = false,
        bool isStarterPressed = false
    );

    void starterButtonBox();
    void ignition();
    void starter();
    void engineRunning();
    void readEngineMode();
    void engineRunningSwitchState();
    bool isIgnitionOnByTelemetry();
    bool isEngineOnByTelemetry();
};

#endif // ENGINESTARTER_H