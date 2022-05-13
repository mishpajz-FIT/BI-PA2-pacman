#include "NCColors.h"

void NCColors::initialize() {
    start_color();
    use_default_colors();

    init_color(Color::darkBlue, 0, 0, 153);
    init_pair(ColorPairs::wall, Color::darkBlue, COLOR_BLACK);
    init_pair(ColorPairs::space, COLOR_BLACK, -1);
    init_pair(ColorPairs::coin, COLOR_WHITE, -1);
    init_pair(ColorPairs::bonus, COLOR_RED, -1);
    init_pair(ColorPairs::pacman, COLOR_YELLOW, -1);
    init_pair(ColorPairs::ghostBlinky, COLOR_RED, -1);
    init_color(Color::pink, 1000, 400, 1000);
    init_pair(ColorPairs::ghostPinky, Color::pink, -1);
    init_color(Color::turqoise, 0, 1000, 1000);
    init_pair(ColorPairs::ghostInky, Color::turqoise, -1);
    init_color(Color::orange, 1000, 600, 0);
    init_pair(ColorPairs::ghostClyde, Color::orange, -1);
    init_color(Color::lightBlue, 200, 400, 1000);
    init_pair(ColorPairs::ghostFrighten, Color::lightBlue, -1);
    init_pair(ColorPairs::basic, -1, -1);
}