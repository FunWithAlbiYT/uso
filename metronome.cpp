#include <metronome.hpp>
#include <iostream>
#include <thread>

BeatTracker::BeatTracker(int beatsPerMinute) : bpm(beatsPerMinute), beatInterval(0.0) {
    startTime = std::chrono::steady_clock::now();
    lastBeatTime = startTime;
    calculateBeatInterval();
}

void BeatTracker::calculateBeatInterval() {
    beatInterval = 60.0 / bpm;
}

bool BeatTracker::isBeat() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration<double>(currentTime - lastBeatTime).count();

    if (elapsedTime >= beatInterval) {
        lastBeatTime = currentTime;
        return true;
    }

    return false;
}