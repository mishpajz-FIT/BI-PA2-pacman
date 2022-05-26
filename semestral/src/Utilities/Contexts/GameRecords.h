/****************************************************************
 * @file GameRecords.h
 * @author Michal Dobes
 * @brief Game records context
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef GAMERECORDS_H
#define GAMERECORDS_H

#include <map>
#include <string>
#include <tuple>

/**
 * @brief Game records
 *
 * Stores previous records for board, manages getiing, adding new record or
 * overwriting previous one
 *
 */
struct GameRecords {
    std::map<std::pair<std::string, unsigned int>, unsigned long> records; //< Map of records
    // key is pair of map file name and difficulty, value is max score reached

    /**
     * @brief Construct a new Game Records object
     *
     */
    GameRecords();

    /**
     * @brief Destroy the Game Records object
     *
     */
    ~GameRecords();

    /**
     * @brief Try to add score as record
     *
     * @param map map file name
     * @param difficulty difficulty of game
     * @param score reached score
     * @return true reached score is a new record and was stored
     * @return false reached score is not record and was discarded
     */
    bool addScore(const std::string & map, unsigned int difficulty, unsigned long score);

};

#endif /* GAMESETTINGS_H */