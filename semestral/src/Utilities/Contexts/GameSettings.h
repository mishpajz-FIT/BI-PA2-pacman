/****************************************************************
 * @file GameSettings.h
 * @author Michal Dobes
 * @brief Game settings context
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

/**
 * @brief Game settings
 *
 * Storage for all game configuration options.
 *
 */
struct GameSettings {
    unsigned int playerSpeed;
    unsigned int enemySpeed;

    unsigned int scatterDuration;
    unsigned int chaseDuration;
    unsigned int frightenDuration;

    unsigned int killDuration;
    unsigned int bonusPeriod;
    unsigned int ghostComeOutPeriod;

    /**
     * @brief Construct a new, nulled Game Settings object
     *
     */
    GameSettings();

    /**
     * @brief Construct a new Game Settings object with parameters
     *
     * @param playerSpd
     * @param enemySpd
     * @param scatterDur
     * @param chaseDur
     * @param frightenDur
     * @param killDur
     * @param bonusPer
     * @param ghostComeOutPer
     */
    GameSettings(
        unsigned int playerSpd,
        unsigned int enemySpd,
        unsigned int scatterDur,
        unsigned int chaseDur,
        unsigned int frightenDur,
        unsigned int killDur,
        unsigned int bonusPer,
        unsigned int ghostComeOutPer
    );

    /**
     * @brief Destroy the Game Settings object
     *
     */
    ~GameSettings();
};

#endif /* GAMESETTINGS_H */