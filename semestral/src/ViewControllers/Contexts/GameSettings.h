#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

struct GameSettings {
    unsigned int playerSpeed;
    unsigned int enemySpeed;

    unsigned int scatterDuration;
    unsigned int chaseDuration;
    unsigned int frightenDuration;

    unsigned int killDuration;
    unsigned int bonusPeriod;
    unsigned int ghostComeOutPeriod;

    GameSettings();

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
};

#endif /* GAMESETTINGS_H */