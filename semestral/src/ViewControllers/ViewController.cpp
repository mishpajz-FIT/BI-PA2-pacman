#include "ViewController.h"
#include "StateManager.h"

ViewController::ViewController() : nextState(AppState::programContinue) { }

ViewController::~ViewController() { }