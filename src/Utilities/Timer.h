/****************************************************************
 * @file Timer.h
 * @author Michal Dobes
 * @brief Timer utility
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <queue>
#include <deque>
#include <stdexcept>

/**
 * @brief Timer
 *
 * Allows creation of actions, which, after given interval, perform specified action on update.
 * Supports repeating actions.
 *
 * Supports pausing.
 *
 */
class Timer {
private:
    // Typedef long names
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::time_point<clock> timepoint;
    typedef std::chrono::milliseconds milliseconds;

    /**
     * @brief Timer action object.
     *
     * Should be stored in priority queue
     *
     */
    class TimerObject {
    private:
        timepoint initializeTime; //< Time of initialization
        milliseconds periodDuration; //< Period time for action after initialization

        std::function<void()> action; //< Action to perform
        bool isRepeatingAction; //< Should repeat after performing action

    public:

        /**
         * @brief Construct a new Timer Object object
         *
         * @param initTime Time of initialization
         * @param perDur Milliseconds, after which to perform action
         * @param act Action to perform
         * @param repeat Repeat after performing action
         */
        TimerObject(timepoint initTime, milliseconds perDur, std::function<void()> act, bool repeat);

        /**
         * @brief Get milliseconds from initalization time, after which to perform action
         *
         * @return milliseconds
         */
        milliseconds getPeriodDuration() const;

        /**
         * @brief Get time of initialization
         *
         * @return timepoint
         */
        timepoint getBeginTime() const;

        /**
         * @brief Get action to perform
         *
         * @return std::function<void()>
         */
        std::function<void()> getAction() const;

        /**
         * @brief Get time at which to perform action
         *
         * @return timepoint
         */
        timepoint actionTime() const;

        /**
         * @brief Perform action
         *
         */
        void callAction();

        /**
         * @brief Is repeating
         *
         * @return true
         * @return false
         */

        bool repeating() const;

        /**
         * @brief Update initalization time to given timepoint
         *
         * @param to
         */
        void updateBeginning(timepoint to);

        /**
         * @brief Comparison operator for priority queue
         *
         * Oldest action times are largest
         *
         * @param rhs
         * @return true
         * @return false
         */
        bool operator < (const TimerObject & rhs) const;
    };

private:
    bool paused;
    timepoint lastPausedTime; //< Time when was paused

    std::priority_queue<TimerObject> timerQueue; //< Priority queue of triggers


public:

    /**
     * @brief Construct a new Timer object
     *
     */
    Timer();

    /**
     * @brief Is timer paused
     *
     * @return true
     * @return false
     */
    bool isPaused();

    /**
     * @brief Toggle pause on/off
     *
     * Trigger times are adjusted by the time timer was paused.
     *
     */
    void togglePause();

    /**
     * @brief Update timer
     *
     * @warning Should be called periodically.
     *
     * Performs all triggers that should be perofrmed by now.
     *
     */
    void update();

    /**
     * @brief Add new trigger
     *
     * @param milliseconds milliseconds after current time to perform action
     * @param action action to perform
     * @param repeating repeat after performing action
     */
    void addTrigger(unsigned int milliseconds, std::function<void()> action, bool repeating = false);

};
#endif /* TIMER_H */
