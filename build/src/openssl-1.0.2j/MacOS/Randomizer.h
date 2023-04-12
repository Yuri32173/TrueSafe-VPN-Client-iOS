#include <chrono>
#include <random>
#include <cstddef>

class Randomizer {
public:
    Randomizer() : supports_large_volumes_(false), is_power_pc_(false), is_601_(false), timebase_ticks_per_millisec_(0.0), last_periodic_ticks_(0), sample_period_(0), mouse_still_(0) {}
    void PeriodicAction() {}

private:
    void AddTimeSinceMachineStartup() {}
    void AddAbsoluteSystemStartupTime() {}
    void AddAppRunningTime() {}
    void AddStartupVolumeInfo() {}
    void AddFiller() {}
    void AddCurrentMouse() {}
    void AddNow(double millisecondUncertainty) {}
    void AddBytes(void *data, std::size_t size, double entropy) {}
    void GetTimeBaseResolution() {}
    unsigned long SysTimer() {}

    // System Info
    bool supports_large_volumes_;
    bool is_power_pc_;
    bool is_601_;

    // Time info
    double timebase_ticks_per_millisec_;
    unsigned long last_periodic_ticks_;

    // Mouse info
    long sample_period_;
    Point last_mouse_;
    long mouse_still_;
};
