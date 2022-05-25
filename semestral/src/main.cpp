/****************************************************************
 * @file main.cpp
 * @author Michal Dobes
 * @brief dobesmic's PacMan
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#include <ncurses.h>
#include <iostream>

#include "Utilities/NCColors.h"
#include "StateManager.h"


int main(void) {
    initscr();
    curs_set(0);
    noecho();
    NCColors::initialize();

    StateManager stateManager;
    stateManager.run();

    endwin();
    return 0;
}