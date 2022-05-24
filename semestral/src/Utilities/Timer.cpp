#include "Utilities/Timer.h"

// SECTION: TimerObject
Timer::TimerObject::TimerObject(timepoint initTime, milliseconds perDur, std::function<void()> act, bool repeat) : initializeTime(initTime), periodDuration(perDur), action(act), isRepeatingAction(repeat) {
    if (perDur.count() == 0 && repeat) {
        throw std::invalid_argument("Timer::TimerObject: TimerObject - repeating action with 0 period");
    }
}

Timer::milliseconds Timer::TimerObject::getPeriodDuration() const {
    return periodDuration;
}

Timer::timepoint Timer::TimerObject::getBeginTime() const {
    return initializeTime;
}

std::function<void()> Timer::TimerObject::getAction() const {
    return action;
}

Timer::timepoint Timer::TimerObject::actionTime() const {
    return (initializeTime + periodDuration);
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
Timer::Timer() : paused(true), lastPausedTime(Timer::clock::now()), timeOffset(0) { }

bool Timer::isPaused() {
    return paused;
}

void Timer::togglePause() {
    if (!paused) {
        lastPausedTime = Timer::clock::now();
    } else {
        std::priority_queue<TimerObject> newQueue;

        while (!timerQueue.empty()) {
            const TimerObject & originalObject = timerQueue.top();
            Timer::milliseconds timePassedInObject = std::chrono::duration_cast<Timer::milliseconds>(lastPausedTime - originalObject.getBeginTime());
            Timer::TimerObject newObject(
                Timer::clock::now() - timePassedInObject,
                originalObject.getPeriodDuration(),
                originalObject.getAction(),
                originalObject.repeating());
            newQueue.push(newObject);
            timerQueue.pop();
        }

        timerQueue = newQueue;
    }

    paused = !paused;
}

void Timer::update() {
    if (paused) {
        return;
    }

    while ((timerQueue.size() != 0) && (timerQueue.top().actionTime() <= Timer::clock::now())) {
        Timer::TimerObject copy(timerQueue.top());
        timerQueue.pop();

        copy.callAction();

        if (copy.repeating()) {
            copy.updateBeginning(Timer::clock::now());
            timerQueue.push(copy);
        }
    }
}

void Timer::addTrigger(unsigned int period, std::function<void()> action, bool repeating) {
    TimerObject newObject(Timer::clock::now(), Timer::milliseconds(period), action, repeating);

    timerQueue.push(newObject);
}
//!SECTION: Timer
