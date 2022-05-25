#include "Views/LayoutView.h"
#include <iostream>

void LayoutView::recreateWindows() {
    removeWindows();

    // Create windows with fixed secondary view size and flexible primary view size
    unsigned int secondaryX = 10;
    if (secondaryView) {
        secondaryX = secondaryView->getMinSizeX();
    }

    primaryWindow = newwin(sizeY, sizeX - secondaryX - 1, 0, 0);
    secondaryWindow = newwin(sizeY, secondaryX, 0, sizeX - secondaryX);
}

LayoutView::LayoutView() : View(), primaryWindow(nullptr), secondaryWindow(nullptr) { }

LayoutView::LayoutView(const LayoutView & toCopy) : View(toCopy), primaryWindow(nullptr), secondaryWindow(nullptr) {
    if (toCopy.primaryView) {
        primaryView.reset(toCopy.getPrimaryView()->clone());
    }
    if (toCopy.secondaryView) {
        secondaryView.reset(toCopy.getSecondaryView()->clone());
    }
}

LayoutView & LayoutView::operator = (const LayoutView & toCopy) {
    removeWindows();
    if (toCopy.getPrimaryView() != nullptr) {
        primaryView.reset(toCopy.getPrimaryView()->clone());
    } else {
        primaryView.release();
    }

    if (toCopy.getSecondaryView() != nullptr) {
        secondaryView.reset(toCopy.getSecondaryView()->clone());
    } else {
        secondaryView.release();
    }

    return *this;
}


LayoutView::~LayoutView() {
    removeWindows();
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

        // Size is smaller than minimum size
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

    if (!isAbleToDisplay()) {
        return;
    }

    if (needsRefresh) {
        clear();
        recreateWindows();
        refresh();
    }

    // Draw primary and secondary view
    if (primaryView) {
        primaryView->draw(primaryWindow);
    }
    
    if (secondaryView) {
        secondaryView->draw(secondaryWindow);
    }

    doupdate(); // Display drawn elements to screen
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

void LayoutView::removeWindows() {
    if (primaryWindow != nullptr) {
        delwin(primaryWindow);
        primaryWindow = nullptr;
    }
    if (secondaryWindow != nullptr) {
        delwin(secondaryWindow);
        primaryWindow = nullptr;
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