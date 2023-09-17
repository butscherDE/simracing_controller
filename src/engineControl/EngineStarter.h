#ifndef ENGINESTARTER_H
#define ENGINESTARTER_H

class EngineStarter {

private:
    int ignitionSwitchPin;
    bool lastIgnitionState;
    bool engineRunningLedState;
    bool isStarterPressed;
    int starterButtonPin;
    int engineMode = 0; // 0 = ignition off, 1 = ignition on, engine off, 2 = starting, 3 = started
public:
    EngineStarter(
        int ignitionSwitchPin,
        int starterButtonPin,
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
    bool isEngineOnByTelemetry();
};

#endif // ENGINESTARTER_H