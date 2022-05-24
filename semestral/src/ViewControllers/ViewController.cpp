#include "ViewController.h"
#include "StateManager.h"

bool ViewController::handleStateExitKey(int c) {
    if (c == 'q' || c == 'Q') {
        nextState = AppState::programExit;
        return true;
    }
    return false;
}

ViewController::ViewController() : nextState(AppState::programContinue) { }

ViewController::~ViewController() { }