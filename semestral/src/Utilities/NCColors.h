/****************************************************************
 * @file NCColors.h
 * @author Michal Dobes
 * @brief Color utility
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef NCCOLORS_H
#define NCCOLORS_H

#include <ncurses.h>

/**
 * @brief Color manager
 *
 * Initializes and defines colors.
 *
 */
class NCColors {
public:
    /**
     * @brief Color Pair definitions
     *
     * Each numeral value is corresponding to value of ncurses color pair.
     *
     */
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

    /**
     * @brief Initialize ncurses colors
     *
     * @warning Needs to be run before displaying anything.
     * @warning If terminal doesn't support colors, exits program.
     *
     * Works best with 256+ colors terminal, has fallback for 8 colors.
     *
     */
    static void initialize();

private:
    /**
     * @brief Extra Color definitios
     *
     * Each numberal value is correspoding to value of ncurses color.
     *
     */
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