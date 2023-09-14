extern int last_rpm;

class FanTacho {
    public:
        FanTacho(int value);
        void initTacho(void);
        void updateTacho(void);

    private:
        int tachoPin;
        static void rpmFan(void);
        const int tachoUpdateCycle = 1000;
        const int numberOfInterrupsInOneSingleRotation = 2;
};
