#include "GameViewController.h"

#include <ncurses.h>
#include <NCColors.h>

int main(void) {
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    NCColors::initialize();

    GameViewController gameViewController;

    while (true) {
        gameViewController.update();
        gameViewController.draw();
    }
}