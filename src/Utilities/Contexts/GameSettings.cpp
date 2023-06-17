#include "Utilities/Contexts/GameSettings.h"

GameSettings::GameSettings() :
    playerSpeed(0),
    enemySpeed(0),
    scatterDuration(0),
    chaseDuration(0),
    frightenDuration(0),
    killDuration(0),
    bonusPeriod(0),
    ghostComeOutPeriod(0) { }

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

GameSettings::~GameSettings() { }