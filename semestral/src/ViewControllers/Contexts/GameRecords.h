#ifndef GAMERECORDS_H
#define GAMERECORDS_H

#include <map>
#include <string>
#include <tuple>

struct GameRecords {
    std::map<std::pair<std::string, unsigned int>, unsigned long> records;

    GameRecords();

    bool addScore(const std::string & map, unsigned int difficulty, unsigned long score);

};

#endif /* GAMESETTINGS_H */