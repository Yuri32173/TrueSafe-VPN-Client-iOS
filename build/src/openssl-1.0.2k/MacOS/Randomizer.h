// Gathers unpredictable system data to be used for generating
// random bits

#include <MacTypes.h>

class Randomizer {
public:
Randomizer();
void periodicAction();

private:

// Private methods

void addTimeSinceMachineStartup();
void addAbsoluteSystemStartupTime();
void addAppRunningTime();
void addStartupVolumeInfo();
void addFiller();

void addCurrentMouse();
void addNow(double millisecondUncertainty);
void addBytes(void* data, long size, double entropy);

void getTimeBaseResolution();
unsigned long sysTimer();

// System Info
bool supportsLargeVolumes;
bool isPowerPC;
bool is601;

// Time info
double timebaseTicksPerMillisec;
unsigned long lastPeriodicTicks;

// Mouse info
long samplePeriod;
Point lastMouse;
long mouseStill;

    unsigned long mLastPeriodicTicks;

    // Mouse info
    long mSamplePeriod;
    Point mLastMouse;
    long mMouseStill;
};
