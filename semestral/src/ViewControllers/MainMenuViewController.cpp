#include "MainMenuViewController.h"
#include "LogoView.h"
#include "OptionMenuView.h"
#include <optional>

void MainMenuViewController::handleMenuSelect(unsigned int i) {
    switch (i) {
        case 0:
            nextState = AppState::game;
            break;
        case 1:
            nextState = AppState::programExit;
            break;
        default:
            break;
    }
}

MainMenuViewController::MainMenuViewController() : ViewController(), layoutView() {
    layoutView.setPrimaryView(LogoView());

    menu.reset(new OptionMenu());
    menu->addOption("play");
    menu->addOption("exit");

    layoutView.setSecondaryView(OptionMenuView(menu.get()));
}

AppState MainMenuViewController::update() {
    keypad(layoutView.getSecondaryWindow(), TRUE);
    int c = wgetch(layoutView.getSecondaryWindow());
    if (c == 'q' || c == 'Q') {
        nextState = AppState::programExit;
        return nextState;
    }

    std::optional<unsigned int> input = menu->handleInput(c);
    if (!input) {
        return nextState;
    }
    handleMenuSelect(*input);

    keypad(layoutView.getSecondaryWindow(), FALSE);
    return nextState;
}

void MainMenuViewController::draw() {
    layoutView.draw();
}