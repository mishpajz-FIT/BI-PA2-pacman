#ifndef GAMEVIEWCONTROLLER_H
#define GAMEVIEWCONTROLLER_H

#include <ncurses.h>
#include <memory>
#include <optional>
#include <string>

#include "ViewControllers/ViewController.h"
#include "GameLogic/Game.h"
#include "Views/LayoutView.h"
#include "Utilities/Contexts/OptionMenu.h"
#include "Utilities/Contexts/GameSettings.h"
#include "Utilities/Contexts/GameRecords.h"

class GameViewController : public ViewController {
protected:
    std::unique_ptr<Game> game;

    enum GameStatePhase {
        difficultyChoosing,
        settingsLoading,
        mapLoading,
        playing,
        endGame
    };
    GameStatePhase phase;

    std::unique_ptr<OptionMenu> menu;

    LayoutView layoutView;

    std::string settingsPath;
    GameSettings loadedSettings;
    GameRecords loadedRecords;
    unsigned int loadedDifficulty;
    std::string mapName;

    bool handleStateExitKey(int c) override;

    void createMenuWithFiles(const std::string & filePath, const std::string & extension);

    void updateDifficultyChoosing();
    void updateSettingsLoading();
    void updateMapLoading();
    void updatePlaying();
    void updateEndGame();

public:
    GameViewController();

    AppState update() override;

    void draw() override;
};

#endif /* GAMEVIEWCONTROLLER_H */
