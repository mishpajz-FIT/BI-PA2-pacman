#include "StateManager.h"
#include "GameViewController.h"

StateManager::StateManager() {
    viewController.reset(new GameViewController());
}

StateManager::~StateManager() { }

void StateManager::run() {
    while (true) {
        AppState nextState = viewController->update();

        if (nextState != AppState::programContinue) {
            return;
        }

        viewController->draw();
    }
}