#include "ViewController.h"

ViewController::ViewController() : termYSize(0), termXSize(0), termSizeChanged(true), problem(false), prevProblem(false), problemMessage("") {
    getTermSizes();
}

void ViewController::getTermSizes() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    if (maxY != termYSize || maxX != termXSize) {
        termSizeChanged = true;
    } else {
        termSizeChanged = false;
    }

    termYSize = maxY;
    termXSize = maxX;
}

void ViewController::boxAronudWindow(WINDOW * win) {
    box(win, 0, 0);
}

void ViewController::wrongResolution() {
    displaying = false;
    printw("Please resize this window");
}

