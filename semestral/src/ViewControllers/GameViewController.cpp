#include "GameViewController.h"
#include "GameSettingsFileLoader.h"
#include "BoardFileLoader.h"
#include "GameDetailView.h"
#include "GameView.h"
#include "OptionsView.h"
#include "LoadingView.h"

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

std::optional<std::string> GameViewController::getInputFromSecondaryView() {
    if (wgetch(layoutView.getSecondaryWindow()) != '\n') {
        return { };
    }

    echo();
    curs_set(1);
    nocbreak();
    char bufferStr[256];
    wrefresh(layoutView.getSecondaryWindow());

    wgetnstr(layoutView.getSecondaryWindow(), bufferStr, 256);
    noecho();
    curs_set(0);
    cbreak();
    return std::string(bufferStr);
}


GameViewController::GameViewController() : ViewController(), game(nullptr), phase(settingsLoading), layoutView() {

    layoutView.setSecondaryView(OptionsView());
    layoutView.getSecondaryView()->setTitle("Enter path to settings file.");

    layoutView.setPrimaryView(LoadingView());

    draw();
}

void GameViewController::update() {
    if (!layoutView.isAbleToDisplay()) {
        if (phase == playing && !game->isPaused()) {
            game->togglePause();
        }
        getch();
        return;
    }

    if (phase == settingsLoading) {
        std::optional<std::string> expectedPath = getInputFromSecondaryView();
        if (!expectedPath) {
            return;
        }
        try {
            GameSettingsFileLoader gameSettingsLoader(*expectedPath);
            game.reset(new Game(gameSettingsLoader.loadSettings(), 1.5));
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
        std::optional<std::string> expectedPath = getInputFromSecondaryView();
        if (!expectedPath) {
            return;
        }
        try {
            game->loadMap(*expectedPath);
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
        game->togglePause();

        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        keypad(stdscr, true);
        return;
    }

    if (phase == playing) {
        if (game->isPaused()) {
            layoutView.getSecondaryView()->setWarning(true, "Paused");
        } else {
            layoutView.getSecondaryView()->setWarning(false);
        }

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