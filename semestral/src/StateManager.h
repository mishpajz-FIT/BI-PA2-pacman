/****************************************************************
 * @file StateManager.h
 * @author Michal Dobes
 * @brief State manager
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
  *****************************************************************/

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <memory>

#include "ViewControllers/ViewController.h"
#include "AppState.h"

/**
 * @brief State manager
 *
 * Has main loop of the program,
 * controls current view controller.
 *
 * App logic flowchart
 * @image html statesmanager.png
 *
 */
class StateManager {
private:
    std::unique_ptr<ViewController> viewController;

    /**
     * @brief Handle recieved state from viewController
     *
     * Changes viewController based on recieved state.
     *
     * @param state recieved state
     */
    void handleState(AppState state);

public:
    /**
     * @brief Construct a new State Manager object
     *
     */
    StateManager();

    /**
     * @brief Destroy the State Manager object
     *
     */
    ~StateManager();

    /**
     * @brief Main App loop
     *
     */
    void run();
};

#endif /* STATEMANAGER_H */