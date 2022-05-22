#include "MainMenuViewController.h"
#include "LogoView.h"
#include "OptionMenuView.h"

void MainMenuViewController::handleMenuSelect() {
    switch (menu->getCurrentOption()) {
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
    switch (c) {
        case 'q':
        case 'Q':
            nextState = AppState::programExit;
            break;
        case KEY_UP:
            menu->changeSelection(false);
            layoutView.getSecondaryView()->setNeedsRefresh();
            break;
        case KEY_DOWN:
            menu->changeSelection(true);
            layoutView.getSecondaryView()->setNeedsRefresh();
            break;
        case '\n':
            handleMenuSelect();
            break;
    }

    keypad(layoutView.getSecondaryWindow(), FALSE);
    return nextState;
}

void MainMenuViewController::draw() {
    layoutView.draw();
}