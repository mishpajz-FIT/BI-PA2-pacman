/****************************************************************
 * @file ViewController.h
 * @author Michal Dobes
 * @brief View controller
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <ncurses.h>
#include <string>

#include "AppState.h"

/**
 * @brief View controller
 *
 * View controller should manage and connect Views and Models.
 *
 */
class ViewController {
protected:
    AppState nextState; //< Next state that should be returned at end of update

    /**
     * @brief Check if inputted key is requesting exit
     *
     * Sets next state to exit state (default is programExit)
     *
     * @param c value of inputted key
     * @return true
     * @return false
     */
    virtual bool handleStateExitKey(int c);

public:

    /**
     * @brief Construct a new View Controller object
     *
     */
    ViewController();

    /**
     * @brief Destroy the View Controller object
     *
     */
    virtual ~ViewController();

    /**
     * @brief Update logic
     *
     * Performs all logic and actions that connect model and view except for drawing to screen.
     *
     * @warning Should be called periodically
     *
     * @return AppState state program should go to next loop
     */
    virtual AppState update() = 0;

    /**
     * @brief Update screen
     *
     * Draws Views on screen.
     *
     */
    virtual void draw() = 0;
};

#endif /* STATE_H */