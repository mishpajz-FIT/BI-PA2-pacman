#include "Timer.h"

// SECTION: TimerObject
Timer::TimerObject::TimerObject(timepoint initTime, milliseconds perTime, std::function<void()> act, bool repeat) : initializeTime(initTime), periodTime(perTime), action(act), isRepeatingAction(repeat) {
    if (perTime.count() == 0 && repeat) {
        throw std::invalid_argument("Timer::TimerObject: TimerObject - repeating action with 0 period");
    }
}

Timer::timepoint Timer::TimerObject::actionTime() const {
    return (initializeTime + periodTime);
}

void Timer::TimerObject::callAction() {
    action();
}

bool Timer::TimerObject::repeating() const {
    return isRepeatingAction;
}

void Timer::TimerObject::updateBeginning(timepoint to) {
    initializeTime = to;
}

bool Timer::TimerObject::operator < (const TimerObject & rhs) const {
    return rhs.actionTime() < actionTime();
}
// !SECTION



// SECTION: Timer
Timer::timepoint Timer::adjustedTime() {
    return (std::chrono::steady_clock::now() - timeOffset);
}

Timer::Timer() : paused(true), lastPausedTime(Timer::clock::now()), timeOffset(0) { }

void Timer::togglePause() {
    if (!paused) {
        lastPausedTime = Timer::clock::now();
    } else {
        timeOffset += std::chrono::duration_cast<Timer::milliseconds>(Timer::clock::now() - lastPausedTime);
    }

    paused = !paused;
}

void Timer::update() {
    if (paused) {
        return;
    }

    while ((timerQueue.size() != 0) && (timerQueue.top().actionTime() <= adjustedTime())) {
        Timer::TimerObject copy(timerQueue.top());
        timerQueue.pop();

        copy.callAction();

        if (copy.repeating()) {
            copy.updateBeginning(adjustedTime());
            timerQueue.push(copy);
        }
    }
}

void Timer::addTrigger(unsigned int period, std::function<void()> action, bool repeating) {
    TimerObject newObject(adjustedTime(), Timer::milliseconds(period), action, repeating);

    timerQueue.push(newObject);
}
//!SECTION: Timer
