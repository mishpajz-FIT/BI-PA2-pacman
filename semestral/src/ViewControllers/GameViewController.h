/****************************************************************
 * @file GameViewController.h
 * @author Michal Dobes
 * @brief Game view controller
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

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

/**
 * @brief Game view controller
 *
 * Controls game view and game model.
 * Handles loading settings and map from files.
 *
 */
class GameViewController : public ViewController {
protected:
    std::unique_ptr<Game> game; //< Game model

    /**
     * @brief Phases of game
     *
     */
    enum GameStatePhase {
        difficultyChoosing,
        settingsLoading,
        mapLoading,
        playing,
        endGame
    };
    GameStatePhase phase; //< Current phase

    std::unique_ptr<OptionMenu> menu; //< Universal menu object

    LayoutView layoutView; //< Main layout view

    std::string settingsPath; //< Path to file from which were settings loaded
    GameSettings loadedSettings; //< Game settings context loaded from file
    GameRecords loadedRecords; //< Game records context loaded from file
    unsigned int loadedDifficulty; //< Set difficulty of game
    std::string mapName; //< Name of file with map

    bool handleStateExitKey(int c) override;

    /**
     * @brief Create OptionMenu with options as files in directory
     *
     * Store the OptionMenu into menu
     *
     * @param filePath Path to directory
     * @param extension Extension that files should have
     */
    void createMenuWithFiles(const std::string & filePath, const std::string & extension);

    /**
     * @brief Update in difficultyChoosing phase
     *
     */
    void updateDifficultyChoosing();

    /**
     * @brief Update in settingsLoading phase
     * 
     */
    void updateSettingsLoading();

    /**
     * @brief Update in mapLoading phase
     * 
     */
    void updateMapLoading();

    /**
     * @brief Update in playing phase
     * 
     */
    void updatePlaying();

    /**
     * @brief Update in endGame phase
     * 
     */
    void updateEndGame();

public:
    /**
     * @brief Construct a new Game View Controller object
     * 
     */
    GameViewController();

    /**
     * @brief Destroy the Game View Controller object
     * 
     */
    ~GameViewController();

    AppState update() override;

    void draw() override;
};

#endif /* GAMEVIEWCONTROLLER_H */
