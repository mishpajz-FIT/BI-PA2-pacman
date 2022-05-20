#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <queue>
#include <deque>
#include <stdexcept>

class Timer {
private:
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::time_point<clock> timepoint;
    typedef std::chrono::milliseconds milliseconds;

    class TimerObject {
    private:
        timepoint initializeTime;
        milliseconds periodDuration;

        std::function<void()> action;
        bool isRepeatingAction;

    public:

        TimerObject(timepoint initTime, milliseconds perDur, std::function<void()> act, bool repeat);

        milliseconds getPeriodDuration() const;

        timepoint getBeginTime() const;

        std::function<void()> getAction() const;

        timepoint actionTime() const;

        void callAction();

        bool repeating() const;

        void updateBeginning(timepoint to);

        bool operator < (const TimerObject & rhs) const;
    };

private:
    bool paused;
    timepoint lastPausedTime;
    milliseconds timeOffset;

    std::priority_queue<TimerObject> timerQueue;


public:
    Timer();

    bool isPaused();

    void togglePause();

    void update();

    void addTrigger(unsigned int milliseconds, std::function<void()> action, bool repeating = false);

};
#endif /* TIMER_H */
