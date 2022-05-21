#include "GameViewController.h"
#include "GameSettingsFileLoader.h"
#include "BoardFileLoader.h"
#include "GameDetailView.h"
#include "GameView.h"
#include "SettingsView.h"
#include "LoadingView.h"
#include "OptionMenuView.h"

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
    WINDOW * secondaryWindow = layoutView.getSecondaryWindow();

    if (wgetch(secondaryWindow) != '\n') {
        return { };
    }

    echo();
    curs_set(1);
    nocbreak();
    char bufferStr[256];
    wattron(secondaryWindow, COLOR_PAIR(NCColors::interactive));
    wrefresh(secondaryWindow);

    wgetnstr(secondaryWindow, bufferStr, 256);
    noecho();
    curs_set(0);
    cbreak();
    wattroff(secondaryWindow, COLOR_PAIR(NCColors::interactive));
    return std::string(bufferStr);
}



void GameViewController::difficultyChoosingUpdate() {
    keypad(layoutView.getSecondaryWindow(), TRUE);
    int c = wgetch(layoutView.getSecondaryWindow());
    switch (c) {
        case KEY_UP:
            menu->changeSelection(false);
            layoutView.getSecondaryView()->setNeedsRefresh();
            return;
        case KEY_DOWN:
            menu->changeSelection(true);
            layoutView.getSecondaryView()->setNeedsRefresh();
            return;
        case '\n':
            break;
        default:
            return;
    }

    keypad(layoutView.getSecondaryWindow(), FALSE);
    phase = settingsLoading;
    layoutView.setSecondaryView(SettingsView());
    layoutView.getSecondaryView()->setTitle("ENTER PATH TO SETTINGS FILE");
    layoutView.getSecondaryView()->setWarning(false);
}

void GameViewController::settingsLoadingUpdate() {
    std::optional<std::string> expectedPath = getInputFromSecondaryView();
    if (!expectedPath) {
        return;
    }
    try {
        GameSettingsFileLoader gameSettingsLoader(*expectedPath);
        unsigned int hp = 0;
        double speedModif = 0.0;
        switch (menu->getCurrentOption()) {
            case 0:
                hp = 5;
                speedModif = 2;
                break;
            case 1:
                hp = 3;
                speedModif = 1.5;
                break;
            case 2:
                hp = 1;
                speedModif = 1.2;
                break;
            default:
                break;
        }

        game.reset(new Game(gameSettingsLoader.loadSettings(), speedModif, hp));
    }
    catch (FileLoaderException & e) {
        layoutView.getSecondaryView()->setWarning(true, "Couldn't load settings file!");
        layoutView.getSecondaryView()->setNeedsRefresh();
        return;
    }
    phase = mapLoading;
    layoutView.getSecondaryView()->setTitle("ENTER PATH TO MAP FILE");
    layoutView.getSecondaryView()->setWarning(false);
}

void GameViewController::mapLoadingUpdate() {
    std::optional<std::string> expectedPath = getInputFromSecondaryView();
    if (!expectedPath) {
        return;
    }
    try {
        game->loadMap(*expectedPath);
    }
    catch (FileLoaderException & e) {
        layoutView.getSecondaryView()->setWarning(true, "Couldn't load map file!");
        layoutView.getSecondaryView()->setNeedsRefresh();
        return;
    }
    phase = playing;
    layoutView.setSecondaryView(GameDetailView(game.get()));
    layoutView.setPrimaryView(GameView(game.get()));
    game->restart();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);
}

void GameViewController::playingUpdate() {
    if (game->isPaused()) {
        layoutView.getSecondaryView()->setWarning(true, "paused");
    } else {
        layoutView.getSecondaryView()->setWarning(false);
    }

    int c = getch();

    if (!(game->isPaused()) && (c == 'p' || c == 'P')) {
        game->togglePause();
    }

    std::optional<Rotation> playerDir;
    if (c != ERR) {
        playerDir = getPlayerRotationFromKey(c);
    }
    game->update(playerDir);
}

void GameViewController::endGameUpdate() {
    //TODO
}


GameViewController::GameViewController() : ViewController(), game(nullptr), phase(difficultyChoosing), menu(nullptr), layoutView() {

    menu.reset(new OptionMenu());
    menu->addOption("easy");
    menu->addOption("medium");
    menu->addOption("hard");
    layoutView.setSecondaryView(OptionMenuView(menu.get()));
    layoutView.getSecondaryView()->setTitle("CHOOSE DIFFICULTY");
    layoutView.getSecondaryView()->setWarning(false);

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

    switch (phase) {
        case playing:
            return playingUpdate();
        case endGame:
            return endGameUpdate();
        case difficultyChoosing:
            return difficultyChoosingUpdate();
        case settingsLoading:
            return settingsLoadingUpdate();
        case mapLoading:
            return mapLoadingUpdate();
        default:
            break;
    }
}

void GameViewController::draw() {
    layoutView.draw();
}