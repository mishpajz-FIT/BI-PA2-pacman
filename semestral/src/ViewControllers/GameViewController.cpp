#include "GameViewController.h"
#include "GameFileLoader.h"
#include "BoardFileLoader.h"
#include "GameDetailView.h"
#include "GameView.h"
#include "OptionsView.h"
#include "LoadingView.h"
#include <string>

std::optional<Rotation> GameViewController::getPlayerRotationFromKey(int c) {
    switch (c) {
        case KEY_UP:
        case 'w':
        case 'W':
            return Rotation(Rotation::Direction::up);
        case KEY_DOWN:
        case 's':
        case 'S':
            return Rotation(Rotation::Direction::down);
        case KEY_LEFT:
        case 'a':
        case 'A':
            return Rotation(Rotation::Direction::left);
        case KEY_RIGHT:
        case 'd':
        case 'D':
            return Rotation(Rotation::Direction::right);
        default:
            break;
    }

    return { };
}


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
        layoutView.setSecondaryView(GameDetailView(game.get()));
        layoutView.setPrimaryView(GameView(game.get()));
        game->restart();
        game->begin();
        return;
    }

    if (phase == playing) {
        if (game->isPaused()) {
            layoutView.getSecondaryView()->setWarning(true, "Paused");
        } else {
            layoutView.getSecondaryView()->setWarning(false);
        }

        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        keypad(stdscr, true);
        int c = getch();
        std::optional<Rotation> playerDir;
        if (c != ERR) {
            playerDir = getPlayerRotationFromKey(c);
        }
        game->update(playerDir);
    }
}

void GameViewController::draw() {
    layoutView.draw();
}