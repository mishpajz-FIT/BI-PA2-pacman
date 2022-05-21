#include <ncurses.h>
#include <NCColors.h>
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