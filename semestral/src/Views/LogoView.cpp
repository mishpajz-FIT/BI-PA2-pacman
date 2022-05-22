#include "LogoView.h"
#include <string>

const char LogoView::logo[LOGOVIEWLOGOHEIGHT][LOGOVIEWLOGOWIDTH] = {
    "    ____  ____  _________ ___  ____  ____ ",
    "   / __ \\/ __ |/ ___/ __ |__ \\/ __ \\/ __ \\",
    "  / /_/ / /_/ / /__/ / / / / / /_/ / / / /",
    " /  ___/\\__/_/\\___/_/ /_/ /_/\\__/_/_/ /_/ ",
    "/_/                                       "
};

LogoView::LogoView() : View() {
    minSizeX = LOGOVIEWLOGOWIDTH + 5;
    minSizeY = LOGOVIEWLOGOHEIGHT + 7;
}

LogoView::~LogoView() { }

void LogoView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    unsigned int yCoord = 4;

    wattron(intoWindow, A_BOLD);
    mvwprintw(intoWindow, yCoord, centeredXInWindow(1), "dobesmic's");
    wattron(intoWindow, A_BOLD);

    for (size_t i = 0; i < LOGOVIEWLOGOHEIGHT; i++) {
        yCoord++;
        mvwprintw(intoWindow, yCoord, centeredXInWindow(1), logo[i]);
    }

    wrefresh(intoWindow);
    needsRefresh = false;
}

LogoView * LogoView::clone() const {
    return new LogoView(*this);
}

