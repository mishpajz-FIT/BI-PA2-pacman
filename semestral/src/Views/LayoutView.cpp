#include "LayoutView.h"

void LayoutView::recreateWindows() {
    clear();
    if (primaryWindow != nullptr) {
        delwin(primaryWindow);
    }
    if (secondaryWindow != nullptr) {
        delwin(secondaryWindow);
    }
    primaryWindow = newwin(sizeY, sizeX - secondaryX - 1, 0, 0);
    secondaryWindow = newwin(sizeY, secondaryX, 0, sizeX - secondaryX);
    refresh();
}

LayoutView::LayoutView(std::function<void()> refreshCallback) : View(refreshCallback), minPrimaryX(0), minPrimaryY(0), secondaryX(50), minSecondaryY(25) { }
LayoutView::~LayoutView() {
    if (primaryWindow != nullptr) {
        delwin(primaryWindow);
    }
    if (secondaryWindow != nullptr) {
        delwin(primaryWindow);
    }
}

void LayoutView::draw(WINDOW *) {
    getWindowSize();
    if (sizeChanged) {
        if ((sizeX < (minPrimaryX + secondaryX))
            || (sizeY < (minPrimaryY > minSecondaryY ? minPrimaryY : minSecondaryY))) {
            ableToDisplay = false;
            clear();

            curs_set(0);
            noecho();

            mvprintw(1, 1, "Please resize the window");
            refresh();
        return;
        } else {
            ableToDisplay = true;
            setNeedsRefresh();
        }
    }

    if (!needsRefresh) {
        return;
    }

    clear();
    recreateWindows();
    needsRefreshCallback();
    needsRefresh = false;
}

WINDOW * LayoutView::getPrimaryWindow() const {
    return primaryWindow;
}

WINDOW * LayoutView::getSecondaryWindow() const {
    return secondaryWindow;
}

void LayoutView::setMinPrimaryWindowDimensions(unsigned int x, unsigned int y) {
    minPrimaryX = x;
    minPrimaryY = y;
}