#include "OptionsView.h"

OptionsView::OptionsView() : View() {
    minSizeY = 25;
    minSizeX = 50;
}

OptionsView::~OptionsView() { }

void OptionsView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    mvwprintw(intoWindow, 1, 1, titleText.c_str());

    if (warningDisplayed) {
        mvwprintw(intoWindow, sizeY - 3, 1, warningText.c_str());
    }

    if (inputEnabled) {
        curs_set(1);
        echo();
        wmove(intoWindow, sizeY - 2, 1);
    } else {
        curs_set(0);
        noecho();
    }

    wrefresh(intoWindow);
    needsRefresh = false;
}

void OptionsView::setWarning(bool to, std::string text) {
    if (warningDisplayed != to || warningText != text) {
        warningDisplayed = to;
        warningText = text;
        setNeedsRefresh();
    }
}

void OptionsView::setTitle(std::string text) {
    if (titleText != text) {
        titleText = text;
        setNeedsRefresh();
    }
}

void OptionsView::setInput(bool to) {
    if (inputEnabled != to) {
        inputEnabled = to;
        setNeedsRefresh();
    }
}

OptionsView * OptionsView::clone() const {
    return new OptionsView(*this);
}
