#ifndef ENGINESTARTER_H
#define ENGINESTARTER_H

#include "../messaging/MessageContainer.h"

class EngineStarter {

private:
    int pinIgnitionSwitch;
    int pinStarterButton;
    bool lastIgnitionState;
    bool engineRunningLedState;
    bool isStarterPressed;
    int engineMode = 0; // 0 = ignition off, 1 = ignition on, engine off, 2 = starting, 3 = started
    MessageContainer* lastMessage; // Pointer to telemetry data struct
    
    void ignition();
    void starter();
    void readEngineMode();
    void engineRunningSwitchState();
    bool isIgnitionOnByTelemetry();
    bool isEngineOnByTelemetry();
public:
    EngineStarter(
        int ignitionSwitchPin,
        int starterButtonPin,
        MessageContainer* lastMessage
    );

    void starterButtonBox();
};

#endif // ENGINESTARTER_H