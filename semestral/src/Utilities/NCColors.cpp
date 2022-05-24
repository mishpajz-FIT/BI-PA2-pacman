#include "NCColors.h"

void NCColors::initialize() {
    start_color();
    use_default_colors();

    if (COLORS > 8) {
        init_color(Color::darkBlue, 0, 0, 153);
        init_color(Color::pink, 1000, 400, 1000);
        init_color(Color::turqoise, 0, 1000, 1000);
        init_color(Color::orange, 1000, 600, 0);
        init_color(Color::lightBlue, 200, 400, 1000);
        init_color(Color::grayBlue, 400, 400, 600);

        init_pair(ColorPairs::wall, Color::darkBlue, COLOR_BLACK);
        init_pair(ColorPairs::space, COLOR_BLACK, -1);
        init_pair(ColorPairs::coin, COLOR_WHITE, -1);
        init_pair(ColorPairs::bonus, COLOR_RED, -1);
        init_pair(ColorPairs::pacman, COLOR_YELLOW, -1);
        init_pair(ColorPairs::ghostBlinky, COLOR_RED, -1);
        init_pair(ColorPairs::ghostPinky, Color::pink, -1);
        init_pair(ColorPairs::ghostInky, Color::turqoise, -1);
        init_pair(ColorPairs::ghostClyde, Color::orange, -1);
        init_pair(ColorPairs::ghostFrighten, Color::lightBlue, -1);
        init_pair(ColorPairs::basic, -1, -1);
        init_pair(ColorPairs::interactive, COLOR_WHITE, Color::grayBlue);
        init_pair(ColorPairs::hint, Color::grayBlue, -1);
    } else {

        init_pair(ColorPairs::wall, COLOR_BLACK, COLOR_BLACK);
        init_pair(ColorPairs::space, COLOR_BLACK, -1);
        init_pair(ColorPairs::coin, COLOR_WHITE, -1);
        init_pair(ColorPairs::bonus, COLOR_RED, -1);
        init_pair(ColorPairs::pacman, COLOR_YELLOW, -1);
        init_pair(ColorPairs::ghostBlinky, COLOR_RED, -1);
        init_pair(ColorPairs::ghostPinky, COLOR_MAGENTA, -1);
        init_pair(ColorPairs::ghostInky, COLOR_CYAN, -1);
        init_pair(ColorPairs::ghostClyde, COLOR_YELLOW, -1);
        init_pair(ColorPairs::ghostFrighten, COLOR_BLUE, -1);
        init_pair(ColorPairs::basic, -1, -1);
        init_pair(ColorPairs::interactive, COLOR_WHITE, COLOR_BLUE);
        init_pair(ColorPairs::hint, COLOR_WHITE, -1);
    }
}