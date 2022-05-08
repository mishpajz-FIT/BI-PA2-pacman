#include "GameViewController.h"

#include <filesystem>
#include <iostream>
#include <ctime>
#include <ncurses.h>

int main(void) {
    initscr();
    curs_set(0);
    start_color();
    use_default_colors();

    std::clock_t c = std::clock();

    GameViewController gameViewController;

    while (std::clock() < c + CLOCKS_PER_SEC * 180) {
        gameViewController.update();

        gameViewController.draw();
    }
}