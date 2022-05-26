/****************************************************************
 * @file Game.h
 * @author Michal Dobes
 * @brief Game model
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include <optional>

#include "Utilities/Timer.h"
#include "GameLogic/Entities/Player.h"
#include "GameLogic/Entities/Ghosts/Ghosts.h"
#include "Utilities/Contexts/GameSettings.h"

/**
 * @brief Game
 *
 * Runs internal game logic.
 *
 */
class Game {
    // Views as friend classes so they have access to internal values needed for
    // effective displaying
    friend class GameView;
    friend class GameDetailView;

private:
    GameSettings settings; //< Settings object containing configuration

    bool needsRedraw; //< Indicator if values that can be displayed have changed
    std::vector<Position> diffRedraw; //< Positions in board that have changed and
    // probably should be redrawn

    Timer timer; //< Timer used for timing action

    std::unique_ptr<Board> board; //< Game board

    std::unique_ptr<Player> player; //< Player entity
    std::vector<std::unique_ptr<Enemy>> ghosts; //< Enemy entities

    const unsigned int enemyIntelligence; //< Setting of game, intelligence of enemies

    unsigned long score; //< Current reached score
    unsigned int lives; //< Lives remaining

    unsigned int killStreak; //< Player's kill streak from beggining of frighten mode

    /**
     * @brief Detect collisions between player, entities and interactable tiles
     *
     * If tile is interactable, interacts with it
     *
     */
    void detectCollisions();

    /**
     * @brief Move player entity
     *
     */
    void movePlayer();

    /**
     * @brief Move enemy entities
     *
     * Enemy in frightened mode moves in different speed, threfore this method needs to differentiate
     * between frightened mode move and normal mode move
     *
     * Enemy moves only if frightened movement matches frighten status of enemy
     *
     * If frightened movement, creates timer trigger for next frightened movement
     *
     * @param fright Frightened movement
     */
    void moveEnemy(bool fright = false);

    /**
     * @brief Toggle scatter mode on/off on all enemy enitites
     *
     */
    void toggleScatter();

    unsigned int frightenActivated; //< Amount of frighten bonuses currently activated
    const double frightenSpeedMultiplier; //< Setting of game, multiplier of enemy speed
    // when in frighten mode

    /**
     * @brief Toggle frighten mode on/off on alive enemy entities
     *
     * Toggles on/off frighten mode on all alive enemy entities that are not in this
     * frighten mode
     *
     * On toggle on creates timer trigger for turning frighten mode off, and creates initial
     * move enemy with frighten true timer trigger
     *
     * Creates trigger in timer to toggle back off.
     * Raises/decreases frightenActivated.
     * Resets killStreak
     *
     * @param on on/off
     */
    void toggleFrighten(bool on);

    /**
     * @brief Tries to create a bonus tile in board
     *
     */
    void createBonus();

public:

    /**
     * @brief Construct a new Game object
     *
     * @param gameSettings Game settings
     * @param frightenMultiplier Enemy speed multiplier in frightened mode setting
     * @param livesAmount Initial lives amount
     * @param enemyLevel Intelligence level of enemies setting
     */
    Game(
        const GameSettings & gameSettings,
        double frightenMultiplier,
        unsigned int livesAmount,
        unsigned int enemyLevel = 1);

    /**
     * @brief Load board which should be used to play in
     *
     * @param map Board in which to play
     */
    void loadBoard(const Board & map);

    /**
     * @brief Restarts game
     *
     * Note that board needs to be loaded using loadMap before.
     *
     * Sets game into paused mode as side effect.
     *
     */
    void restart();

    /**
     * @brief Update game
     *
     * Should be called periodically in loop.
     *
     * Performs all game actions for which the time has come.
     *
     * Sets player's next movement direction from parameter.
     *
     * @param keyPressDirection std::optional<Rotation> direction of player's next movement
     */
    void update(std::optional<Rotation> keyPressDirection);

    /**
     * @brief Size of x dimension of game board
     *
     * @return unsigned int
     */
    unsigned int getDimensionX();

    /**
     * @brief Size of y dimension of game board
     *
     * @return unsigned int
     */
    unsigned int getDimensionY();

    /**
     * @brief Pause/unpause game
     *
     */
    void togglePause();

    /**
     * @brief Is game paused
     *
     * @return true
     * @return false
     */
    bool isPaused();

    /**
     * @brief Get current score
     *
     * @return unsigned long
     */
    unsigned long getScore();

    /**
     * @brief Get current lives remaining
     *
     * @return unsigned int
     */
    unsigned int getLives();

    /**
     * @brief Get coins in game board remaining
     *
     * @return unsigned int
     */
    unsigned int getCoinsRemaining();

    /**
     * @brief Have values that can be displayed changed
     *
     * @return true
     * @return false
     */
    bool doesNeedRefresh();
};

#endif /* GAME_H */