#include <filesystem>

#include "ViewControllers/GameViewController.h"
#include "Utilities/FileManagers/GameSettingsRecordsFileLoader.h"
#include "Utilities/FileManagers/GameSettingsRecordsFileSaver.h"
#include "Views/SecondaryViews/GameDetailView.h"
#include "Views/SecondaryViews/OptionMenuView.h"
#include "Views/SecondaryViews/SettingsView.h"
#include "Views/GameView.h"
#include "Views/LoadingView.h"
#include "Views/GameOverView.h"

#define SETTINGSPATH "./examples/Settings/"
#define SETTINGSEXTENSION ".spac"
#define MAPSPATH "./examples/Maps/"
#define MAPSEXTENSION ".mpac"


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

bool GameViewController::handleStateExitKey(int c) {
    if (ViewController::handleStateExitKey(c)) {
        nextState = AppState::mainmenu;
        keypad(stdscr, FALSE);
        return true;
    }
    return false;
}

void GameViewController::createMenuWithFiles(const std::string & filePath, const std::string & extension) {
    menu.reset(new OptionMenu());

    try {
        for (const auto & file : std::filesystem::directory_iterator(filePath)) {
            if (file.path().extension() == extension) {
                menu->addOption(file.path().filename());
            }
        }
        layoutView.setSecondaryView(OptionMenuView(menu.get()));
    }
    catch (std::filesystem::filesystem_error & e) {
        layoutView.setSecondaryView(SettingsView(false));
        layoutView.getSecondaryView()->setWarning(true, "Couldn't open directory!");
        return;
    }

    if (menu->size() == 0) {
        layoutView.setSecondaryView(SettingsView(false));
        layoutView.getSecondaryView()->setWarning(true, "No file found!");
    }
}

void GameViewController::difficultyChoosingUpdate() {
    keypad(layoutView.getSecondaryWindow(), TRUE);
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);

    if (handleStateExitKey(c)) {
        return;
    }

    std::optional<unsigned int> difficulty = menu->handleInput(c);
    if (!difficulty) {
        return;
    }

    loadedDifficulty = *difficulty;

    phase = settingsLoading;
    createMenuWithFiles(SETTINGSPATH, SETTINGSEXTENSION);
    layoutView.getSecondaryView()->setTitle("CHOOSE SETTINGS FILE");
}

void GameViewController::settingsLoadingUpdate() {
    keypad(layoutView.getSecondaryWindow(), TRUE);
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);

    if (handleStateExitKey(c)) {
        return;
    }
    if (!menu->handleInput(c)) {
        return;
    }

    settingsPath = SETTINGSPATH;
    settingsPath += menu->getCurrentOptionName();

    try {
        GameSettingsRecordsFileLoader gameSettingsLoader(settingsPath);
        unsigned int hp = 0;
        double speedModif = 0.0;
        switch (loadedDifficulty) {
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

        std::pair<GameSettings, GameRecords> loadedSettingsAndRecords = gameSettingsLoader.loadSettingsAndRecords();
        game.reset(new Game(loadedSettingsAndRecords.first, speedModif, hp, loadedDifficulty));

        loadedSettings = loadedSettingsAndRecords.first;
        loadedRecords = loadedSettingsAndRecords.second;
    }
    catch (FileLoaderException & e) {
        layoutView.getSecondaryView()->setWarning(true, "Couldn't load settings file!");
        layoutView.getSecondaryView()->setNeedsRefresh();
        return;
    }

    phase = mapLoading;
    createMenuWithFiles(MAPSPATH, MAPSEXTENSION);
    layoutView.getSecondaryView()->setTitle("CHOOSE MAP FILE");
}

void GameViewController::mapLoadingUpdate() {
    keypad(layoutView.getSecondaryWindow(), TRUE);
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);

    if (handleStateExitKey(c)) {
        return;
    }
    if (!menu->handleInput(c)) {
        return;
    }

    mapName = menu->getCurrentOptionName();
    std::string mapPath = MAPSPATH;
    mapPath += mapName;

    try {
        game->loadMap(mapPath);
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
    keypad(stdscr, TRUE);
}

void GameViewController::playingUpdate() {
    nodelay(stdscr, TRUE);

    int c = getch();

    if (!(game->isPaused()) && (c == 'p' || c == 'P')) {
        game->togglePause();
    }

    if (game->isPaused()) {
        layoutView.getSecondaryView()->setWarning(true, "paused");
    } else {
        layoutView.getSecondaryView()->setWarning(false);
    }

    if (game->isPaused() && handleStateExitKey(c)) {
        return;
    }

    std::optional<Rotation> playerDir;
    if (c != ERR) {
        playerDir = getPlayerRotationFromKey(c);
    }

    game->update(playerDir);

    if (game->getLives() == 0 || game->getCoinsRemaining() == 0) {
        layoutView.setSecondaryView(SettingsView(false));
        layoutView.getSecondaryView()->setTitle("GAME OVER");
        bool highscore = loadedRecords.addScore(mapName, loadedDifficulty, game->getScore());
        layoutView.setPrimaryView(GameOverView(game->getScore(), loadedRecords.records[std::make_pair(mapName, loadedDifficulty)], highscore));

        try {
            GameSettingsRecordsFileSaver saver(settingsPath);
            saver.writeSettingsAndRecords(std::make_pair(loadedSettings, loadedRecords));
        }
        catch (FileLoaderException & e) {
            layoutView.getSecondaryView()->setWarning(true, "Couldn't save settings and records");
        }

        phase = endGame;
    }

    nodelay(stdscr, FALSE);
}

void GameViewController::endGameUpdate() {
    int c = getch();
    handleStateExitKey(c);
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
}

AppState GameViewController::update() {
    if (!layoutView.isAbleToDisplay()) {
        if (phase == playing && !game->isPaused()) {
            game->togglePause();
        }
        getch();
        return AppState::programContinue;
    }

    switch (phase) {
        case playing:
            playingUpdate();
            break;
        case endGame:
            endGameUpdate();
            break;
        case difficultyChoosing:
            difficultyChoosingUpdate();
            break;
        case settingsLoading:
            settingsLoadingUpdate();
            break;
        case mapLoading:
            mapLoadingUpdate();
            break;
        default:
            break;
    }
    return nextState;
}

void GameViewController::draw() {
    layoutView.draw();
}