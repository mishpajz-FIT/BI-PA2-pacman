#ifndef GAMEVIEWCONTROLLER_H
#define GAMEVIEWCONTROLLER_H

#include "ViewController.h"
#include "Game.h"
#include <memory>
#include <ncurses.h>
#include "LayoutView.h"
#include "OptionMenu.h"
#include <optional>
#include <string>
#include "GameSettings.h"
#include "GameRecords.h"

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

    std::optional<Rotation> getPlayerRotationFromKey(int c);

    void createMenuWithFiles(const std::string & filePath, const std::string & extension);

    void difficultyChoosingUpdate();
    void settingsLoadingUpdate();
    void mapLoadingUpdate();
    void playingUpdate();
    void endGameUpdate();

public:
    GameViewController();

    AppState update() override;

    void draw() override;
};

#endif /* GAMEVIEWCONTROLLER_H */
