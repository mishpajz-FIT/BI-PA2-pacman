#ifndef NCCOLORS_H
#define NCCOLORS_H

#include <ncurses.h>

class NCColors {
public:
    enum ColorPairs : unsigned int {
        wall = 10,
        space,
        coin,
        bonus,
        pacman,
        ghostBlinky,
        ghostPinky,
        ghostInky,
        ghostClyde,
        ghostFrighten,
        basic
    };

    static void initialize();

private:
    enum Color : unsigned int {
        pink = 10,
        turqoise,
        orange,
        darkBlue,
        lightBlue
    };
};

#endif /* NCCOLORS_H */