#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <memory>

#include "ViewControllers/ViewController.h"
#include "AppState.h"

class StateManager {
private:
    std::unique_ptr<ViewController> viewController;

    void handleState(AppState state);

public:
    StateManager();
    ~StateManager();

    void run();
};

#endif /* STATEMANAGER_H */