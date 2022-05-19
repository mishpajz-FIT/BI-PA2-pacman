#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

struct GameSettings {
    const unsigned int playerSpeed;
    const unsigned int enemySpeed;

    const unsigned int scatterDuration;
    const unsigned int chaseDuration;
    const unsigned int frightenDuration;

    const unsigned int killDuration;
    const unsigned int bonusPeriod;
    const unsigned int ghostComeOutPeriod;

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