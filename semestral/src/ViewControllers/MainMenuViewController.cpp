#include <optional>

#include "ViewControllers/MainMenuViewController.h"
#include "Views/LogoView.h"
#include "Views/SecondaryViews/OptionMenuView.h"

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

MainMenuViewController::~MainMenuViewController() { }

AppState MainMenuViewController::update() {
    if (!layoutView.isAbleToDisplay()) {
        getch();
        return AppState::programContinue;
    }

    keypad(layoutView.getSecondaryWindow(), TRUE);
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);
    if (handleStateExitKey(c)) {
        return nextState;
    }

    std::optional<unsigned int> input = menu->handleInput(c);
    if (!input) {
        return nextState;
    }
    handleMenuSelect(*input);

    return nextState;
}

void MainMenuViewController::draw() {
    layoutView.draw();
}