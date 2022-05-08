#include "GameViewController.h"
#include "GameFileLoader.h"
#include "BoardFileLoader.h"
#include <string>

GameViewController::GameViewController() : ViewController(), game(nullptr), phase(optionsLoading), layoutView([ this ]() { this->setNeedsRefreshToSubviews(); }), gameView(), optionsView() {
    optionsView.setTitle("Enter path to settings file.");
    optionsView.setInput(true);

    draw();
}

void GameViewController::update() {
    if (!layoutView.isAbleToDisplay()) {
        return;
    }

    if (phase == optionsLoading) {
        char bufferStr[256];
        wgetnstr(layoutView.getSecondaryWindow(), bufferStr, 256);
        std::string expectedPath(bufferStr);
        try {
            GameFileLoader gameLoader(expectedPath);
            game.reset(new Game(gameLoader.loadGame()));
            phase = mapLoading;
            optionsView.setTitle("Enter path to map file");
            optionsView.setWarning(false);
        }
        catch (FileLoaderException & e) {
            optionsView.setWarning(true, "Couldn't load settings file");
        }
        optionsView.setNeedsRefresh();
        return;
    }

    if (phase == mapLoading) {
        char bufferStr[256];
        wgetnstr(layoutView.getSecondaryWindow(), bufferStr, 256);
        std::string expectedPath(bufferStr);
        try {
            game->loadMap(expectedPath);
            phase = playing;
            optionsView.setTitle("PacMan");
            optionsView.setWarning(false);
            optionsView.setInput(false);
            gameView.setGameToDraw(game.get());
            layoutView.setMinPrimaryWindowDimensions(game->getDimensionX(), game->getDimensionY());
        }
        catch (FileLoaderException & e) {
            optionsView.setWarning(true, "Couldn't load map file");
        }
        optionsView.setNeedsRefresh();
        return;
    }

    if (phase == playing) {
        game->update();
    }
}


void GameViewController::draw() {
    layoutView.draw();
    if (layoutView.isAbleToDisplay()) {
        gameView.draw(layoutView.getPrimaryWindow());
        optionsView.draw(layoutView.getSecondaryWindow());
    }
}

void GameViewController::setNeedsRefreshToSubviews() {
    gameView.setNeedsRefresh();
    optionsView.setNeedsRefresh();
}