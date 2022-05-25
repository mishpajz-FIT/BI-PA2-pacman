#include <unistd.h>

#include "StateManager.h"
#include "ViewControllers/GameViewController.h"
#include "ViewControllers/MainMenuViewController.h"

#define STATEMANAGERLOOPDELAY 100000

void StateManager::handleState(AppState state) {
    switch (state) {
        case AppState::mainmenu:
            viewController.reset(new MainMenuViewController());
            break;
        case AppState::game:
            viewController.reset(new GameViewController());
            break;
        default:
            break;
    }
}

StateManager::StateManager() {
    viewController.reset(new MainMenuViewController());
}

StateManager::~StateManager() { }

void StateManager::run() {
    while (true) {
        viewController->draw();
        AppState nextState = viewController->update();

        if (nextState == AppState::programContinue) {
            continue;
        } else if (nextState == AppState::programExit) {
            return;
        } else {
            handleState(nextState);
        }

        usleep(100000);
    }
}