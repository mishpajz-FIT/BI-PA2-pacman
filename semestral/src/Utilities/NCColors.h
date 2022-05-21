#ifndef NCCOLORS_H
#define NCCOLORS_H

#include <ncurses.h>

class NCColors {
public:
    enum ColorPairs : unsigned int {
        wall = 20,
        space,
        coin,
        bonus,
        pacman,
        ghostBlinky,
        ghostPinky,
        ghostInky,
        ghostClyde,
        ghostFrighten,
        basic,
        interactive,
        hint
    };

    static void initialize();

private:
    enum Color : unsigned int {
        pink = 20,
        turqoise,
        orange,
        darkBlue,
        lightBlue,
        grayBlue
    };
};

#endif /* NCCOLORS_H */