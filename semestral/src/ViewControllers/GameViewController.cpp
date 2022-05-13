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
    char bufferStr[256];
    if (!layoutView.isAbleToDisplay()) {
        if (phase == playing && !game->isPaused()) {
            game->togglePause();
        }
        getch();
        return;
    }

    if (phase == optionsLoading) {
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
            game->togglePause();
        }
        catch (FileLoaderException & e) {
            optionsView.setWarning(true, "Couldn't load map file");
        }
        optionsView.setNeedsRefresh();
        return;
    }

    if (phase == playing) {
        if (game->isPaused()) {
            optionsView.setWarning(true, "Paused");
        } else {
            optionsView.setWarning(false);
        }
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