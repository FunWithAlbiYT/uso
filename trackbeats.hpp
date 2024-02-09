#ifndef BEAT_TRACKER_HPP
#define BEAT_TRACKER_HPP

#include <chrono>

class BeatTracker {
private:
    int bpm;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastBeatTime;
    double beatInterval;

public:
    BeatTracker(int beatsPerMinute);
    void calculateBeatInterval();
    bool isBeat();
};

#endif