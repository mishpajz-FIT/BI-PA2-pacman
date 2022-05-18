#include "GameViewController.h"
#include "GameFileLoader.h"
#include "BoardFileLoader.h"
#include "GameDetailView.h"
#include "GameView.h"
#include "OptionsView.h"
#include "LoadingView.h"
#include <string>

GameViewController::GameViewController() : ViewController(), game(nullptr), phase(optionsLoading), layoutView() {

    layoutView.setSecondaryView(OptionsView());
    layoutView.getSecondaryView()->setTitle("Enter path to settings file.");
    layoutView.getSecondaryView()->setInput(true);

    layoutView.setPrimaryView(LoadingView());

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
        }
        catch (FileLoaderException & e) {
            layoutView.getSecondaryView()->setWarning(true, "Couldn't load settings file");
            layoutView.getSecondaryView()->setNeedsRefresh();
            return;
        }
        phase = mapLoading;
        layoutView.getSecondaryView()->setTitle("Enter path to map file");
        layoutView.getSecondaryView()->setWarning(false);
        return;
    }

    if (phase == mapLoading) {
        wgetnstr(layoutView.getSecondaryWindow(), bufferStr, 256);
        std::string expectedPath(bufferStr);
        try {
            game->loadMap(expectedPath);
        }
        catch (FileLoaderException & e) {
            layoutView.getSecondaryView()->setWarning(true, "Couldn't load map file");
            layoutView.getSecondaryView()->setNeedsRefresh();
            return;
        }
        phase = playing;
        layoutView.setSecondaryView(GameDetailView());
        layoutView.setPrimaryView(GameView(game.get()));
        game->start();
        return;
    }

    if (phase == playing) {
        if (game->isPaused()) {
            layoutView.getSecondaryView()->setWarning(true, "Paused");
        } else {
            layoutView.getSecondaryView()->setWarning(false);
        }
        game->update();
    }
}


void GameViewController::draw() {
    layoutView.draw();
}