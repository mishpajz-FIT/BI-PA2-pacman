#include <filesystem>

#include "ViewControllers/GameViewController.h"
#include "Utilities/Contexts/GameControl.h"
#include "Utilities/FileManagers/GameSettingsRecordsFileLoader.h"
#include "Utilities/FileManagers/GameSettingsRecordsFileSaver.h"
#include "Utilities/FileManagers/BoardFileLoader.h"
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


bool GameViewController::handleStateExitKey(int c) {
    if (ViewController::handleStateExitKey(c)) {
        nextState = AppState::mainmenu; //< Return to main menu on exit
        keypad(stdscr, FALSE);
        return true;
    }
    return false;
}

void GameViewController::createMenuWithFiles(const std::string & filePath, const std::string & extension) {
    menu.reset(new OptionMenu());

    try {
        // Add each file in directory with extension as option in menu
        for (const auto & file : std::filesystem::directory_iterator(filePath)) {
            if (file.path().extension() == extension) {
                menu->addOption(file.path().filename());
            }
        }
        layoutView.setSecondaryView(OptionMenuView(menu.get()));
    }
    catch (std::filesystem::filesystem_error & e) { // If problem with directory, set warning
        layoutView.setSecondaryView(SettingsView(false));
        layoutView.getSecondaryView()->setWarning(true, "Couldn't open directory!");
        return;
    }

    if (menu->size() == 0) { // If directory empty, set warning
        layoutView.setSecondaryView(SettingsView(false));
        layoutView.getSecondaryView()->setWarning(true, "No file found!");
    }
}

void GameViewController::updateDifficultyChoosing() {
    keypad(layoutView.getSecondaryWindow(), TRUE); //< Enable keypad (could be disabled if resized)
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);

    if (handleStateExitKey(c)) {
        return;
    }

    // Try to get confirmation from menu, set difficulty from menu selection
    std::optional<unsigned int> difficulty = menu->handleInput(c);
    if (!difficulty) {
        return;
    }

    loadedDifficulty = *difficulty;

    // Prepare next phase
    phase = settingsLoading;
    createMenuWithFiles(SETTINGSPATH, SETTINGSEXTENSION);
    layoutView.getSecondaryView()->setTitle("CHOOSE SETTINGS FILE");
}

void GameViewController::updateSettingsLoading() {
    keypad(layoutView.getSecondaryWindow(), TRUE); //< Enable keypad (could be disabled if resized)
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);

    if (handleStateExitKey(c)) {
        return;
    }
    if (!menu->handleInput(c)) {
        return;
    }

    // Try to load selected file as game settings
    settingsPath = SETTINGSPATH;
    settingsPath += menu->getCurrentOptionName();

    try {
        GameSettingsRecordsFileLoader gameSettingsLoader(settingsPath);
        unsigned int hp = 0;
        double speedModif = 0.0;
        // Different values based on selected difficulty
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

        // Create game from retrieved game settings
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

    // Prepare next phase
    phase = mapLoading;
    createMenuWithFiles(MAPSPATH, MAPSEXTENSION);
    layoutView.getSecondaryView()->setTitle("CHOOSE MAP FILE");
}

void GameViewController::updateMapLoading() {
    keypad(layoutView.getSecondaryWindow(), TRUE); //< Enable keypad (could be disabled if resized)
    int c = wgetch(layoutView.getSecondaryWindow());
    keypad(layoutView.getSecondaryWindow(), FALSE);

    if (handleStateExitKey(c)) {
        return;
    }
    if (!menu->handleInput(c)) {
        return;
    }

    // Try to load selected file as map settings
    mapName = menu->getCurrentOptionName();
    std::string mapPath = MAPSPATH;
    mapPath += mapName;

    try {
        BoardFileLoader mapLoader(mapPath);
        game->loadBoard(mapLoader.loadBoard());
    }
    catch (FileLoaderException & e) {
        layoutView.getSecondaryView()->setWarning(true, "Couldn't load map file!");
        layoutView.getSecondaryView()->setNeedsRefresh();
        return;
    }

    // Prepare next phase
    phase = playing;
    layoutView.setSecondaryView(GameDetailView(game.get()));
    layoutView.setPrimaryView(GameView(game.get()));
    game->restart();
    keypad(stdscr, TRUE);
}

void GameViewController::updatePlaying() {
    nodelay(stdscr, TRUE); //< Set to non-blocking input reading

    int c = getch();

    // Toggle pause if pressed pause button and unpaused
    if (!(game->isPaused()) && (c == 'p' || c == 'P')) {
        game->togglePause();
    }

    // Set warning if paused
    if (game->isPaused()) {
        layoutView.getSecondaryView()->setWarning(true, "paused");
    } else {
        layoutView.getSecondaryView()->setWarning(false);
    }

    if (game->isPaused() && handleStateExitKey(c)) {
        return;
    }

    // Try to interpret input as player rotation
    std::optional<Rotation> playerDir;
    if (c != ERR) {
        playerDir = GameControl::getPlayerRotationFromKey(c);
    }

    // Pass input and update game
    game->update(playerDir);

    // Check if game ended
    if (game->getLives() == 0 || game->getCoinsRemaining() == 0) {
        layoutView.setSecondaryView(SettingsView(false));
        layoutView.getSecondaryView()->setTitle("GAME OVER");
        // Check if new highscore
        bool highscore = loadedRecords.addScore(mapName, loadedDifficulty, game->getScore());
        layoutView.setPrimaryView(
            GameOverView(
                game->getScore(),
                loadedRecords.records[std::make_pair(mapName, loadedDifficulty)],
                highscore
            )
        );

        // Try to save settings together with records back into original file
        try {
            GameSettingsRecordsFileSaver saver(settingsPath);
            saver.writeSettingsAndRecords(std::make_pair(loadedSettings, loadedRecords));
        }
        catch (FileLoaderException & e) {
            layoutView.getSecondaryView()->setWarning(true, "Couldn't save settings and records");
        }

        // Prepare next phase
        phase = endGame;
    }

    nodelay(stdscr, FALSE);
}

void GameViewController::updateEndGame() {
    // Wait only for exit key
    int c = getch();
    handleStateExitKey(c);
}

GameViewController::GameViewController()
    :
    ViewController(),
    game(nullptr),
    phase(difficultyChoosing),
    menu(nullptr),
    layoutView() {

    // Prepare difficultyChoosing phase
    menu.reset(new OptionMenu());
    menu->addOption("easy");
    menu->addOption("medium");
    menu->addOption("hard");
    layoutView.setSecondaryView(OptionMenuView(menu.get()));
    layoutView.getSecondaryView()->setTitle("CHOOSE DIFFICULTY");
    layoutView.getSecondaryView()->setWarning(false);

    layoutView.setPrimaryView(LoadingView());
}

GameViewController::~GameViewController() { }


AppState GameViewController::update() {
    // If unable to display pause game
    if (!layoutView.isAbleToDisplay()) {
        if (phase == playing && !game->isPaused()) {
            game->togglePause();
        }
        getch();
        return AppState::programContinue;
    }

    // Call correct update function based on phase
    switch (phase) {
        case playing:
            updatePlaying();
            break;
        case endGame:
            updateEndGame();
            break;
        case difficultyChoosing:
            updateDifficultyChoosing();
            break;
        case settingsLoading:
            updateSettingsLoading();
            break;
        case mapLoading:
            updateMapLoading();
            break;
        default:
            break;
    }
    return nextState;
}

void GameViewController::draw() {
    layoutView.draw();
}