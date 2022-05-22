#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "ViewController.h"
#include "AppState.h"
#include <memory>

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