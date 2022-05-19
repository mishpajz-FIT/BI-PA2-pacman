#include "GameSettings.h"

GameSettings::GameSettings(
    unsigned int playerSpd,
    unsigned int enemySpd,
    unsigned int scatterDur,
    unsigned int chaseDur,
    unsigned int frightenDur,
    unsigned int killDur,
    unsigned int bonusPer,
    unsigned int ghostComeOutPer
) :
    playerSpeed(playerSpd),
    enemySpeed(enemySpd),
    scatterDuration(scatterDur),
    chaseDuration(chaseDur),
    frightenDuration(frightenDur),
    killDuration(killDur),
    bonusPeriod(bonusPer),
    ghostComeOutPeriod(ghostComeOutPer) { }