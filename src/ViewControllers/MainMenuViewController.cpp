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

    // Prepare menu
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

    if (layoutView.getSecondaryView() == nullptr) {
        return nextState;
    }

    keypad(layoutView.getSecondaryWindow(), TRUE); //< Enable keypad (could be disabled if resized)
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);
    if (handleStateExitKey(c)) {
        return nextState;
    }

    // Try to handle input as menu selection
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