#include "OptionsView.h"

OptionsView::OptionsView() : View(), warningDisplayed(false), warningText(""), titleText(""), inputting(false) { }

OptionsView::~OptionsView() { }

void OptionsView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (inputting) {
        curs_set(1);
        echo();
    } else {
        curs_set(0);
        noecho();
    }

    if (!needsRefresh) {
        return;
    }

    box(intoWindow, 0, 0);

    mvwprintw(intoWindow, 1, 1, titleText.c_str());

    if (warningDisplayed) {
        mvprintw(10, 1, warningText.c_str());
    }

    if (inputting) {
        move(sizeY - 1, 1);
    }

    needsRefresh = false;
}

void OptionsView::setWarning(bool to, std::string text) {
    warningDisplayed = to;
    warningText = text;
    setNeedsRefresh();
}

void OptionsView::setTitle(std::string text) {
    titleText = text;
    setNeedsRefresh();
}

void OptionsView::setInput(bool to) {
    inputting = to;
    setNeedsRefresh();
}