#include "LayoutView.h"

void LayoutView::recreateWindows() {
    clear();
    if (primaryWindow != nullptr) {
        delwin(primaryWindow);
    }
    if (secondaryWindow != nullptr) {
        delwin(secondaryWindow);
    }

    unsigned int secondaryX = 10;
    if (secondaryView) {
        secondaryX = secondaryView->getMinSizeX();
    }

    primaryWindow = newwin(sizeY, sizeX - secondaryX - 1, 0, 0);
    secondaryWindow = newwin(sizeY, secondaryX, 0, sizeX - secondaryX);
    refresh();
}

LayoutView::LayoutView() : View() { }
LayoutView::LayoutView(const LayoutView & toCopy) : View(toCopy), primaryWindow(nullptr), primaryView(nullptr), secondaryWindow(nullptr), secondaryView(nullptr) {
    if (toCopy.primaryView) {
        primaryView.reset(toCopy.getPrimaryView()->clone());
    }
    if (toCopy.secondaryView) {
        secondaryView.reset(toCopy.getSecondaryView()->clone());
    }
}

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
    if (sizeChanged || needsRefresh) {
        minSizeX = 10;
        minSizeY = 10;
        if (primaryView) {
            minSizeX = primaryView->getMinSizeX();
            minSizeY = primaryView->getMinSizeY();
        }
        if (secondaryView) {
            minSizeX += secondaryView->getMinSizeX();
            if (minSizeY < secondaryView->getMinSizeY()) {
                minSizeY = secondaryView->getMinSizeY();
            }
        }
        minSizeX++;
        minSizeY++;

        if ((sizeX < minSizeX) || (sizeY < minSizeY)) {
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

    if (needsRefresh) {
        clear();
        recreateWindows();
    }

    if (primaryView) {
        primaryView->draw(primaryWindow);
    }
    
    if (secondaryView) {
        secondaryView->draw(secondaryWindow);
    }

    needsRefresh = false;
}

void LayoutView::setNeedsRefresh() {
    View::setNeedsRefresh();
    if (primaryView) {
        primaryView->setNeedsRefresh();
    }
    if (secondaryView) {
        secondaryView->setNeedsRefresh();
    }
}

WINDOW * LayoutView::getPrimaryWindow() const {
    return primaryWindow;
}

WINDOW * LayoutView::getSecondaryWindow() const {
    return secondaryWindow;
}

View * LayoutView::getPrimaryView() const {
    return primaryView.get();
}

View * LayoutView::getSecondaryView() const {
    return secondaryView.get();
}

void LayoutView::setPrimaryView(const View & view) {
    primaryView.reset(view.clone());
    setNeedsRefresh();
}

void LayoutView::setSecondaryView(const View & view) {
    secondaryView.reset(view.clone());
    setNeedsRefresh();
}

LayoutView * LayoutView::clone() const {
    return new LayoutView(*this);
}