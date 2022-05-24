#include "Utilities/Contexts/GameRecords.h"

GameRecords::GameRecords() { }

bool GameRecords::addScore(const std::string & map, unsigned int difficulty, unsigned long score) {
    std::pair<std::string, unsigned int> key(map, difficulty);

    if (records[key] >= score) {
        return false;
    }

    records[key] = score;

    return true;
}