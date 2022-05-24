#include "Views/SecondaryViews/SettingsView.h"
#include "Utilities/NCColors.h"

void SettingsView::drawHint(WINDOW * intoWindow) {
    wattron(intoWindow, COLOR_PAIR(NCColors::ColorPairs::hint));
    if (inputable) {
        mvwprintw(intoWindow, sizeY - 7, 1, "enter - input");
    }
    mvwprintw(intoWindow, sizeY - 6, 1, "q - quit");
    wattroff(intoWindow, COLOR_PAIR(NCColors::ColorPairs::hint));
}

SettingsView::SettingsView(bool input) : SecondaryView(), inputable(input) { }

SettingsView::~SettingsView() { }

void SettingsView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    wattron(intoWindow, A_BOLD);
    mvwprintw(intoWindow, 1, centeredXForText(titleText), titleText.c_str());
    wattroff(intoWindow, A_BOLD);

    drawHint(intoWindow);

    if (warningDisplayed) {
        wattron(intoWindow, A_DIM);
        mvwprintw(intoWindow, sizeY - 3, 1, warningText.c_str());
        wattroff(intoWindow, A_DIM);
    }

    if (inputable) {
        wattron(intoWindow, COLOR_PAIR(NCColors::interactive));
        for (size_t i = 1; i < sizeX - 1; i++) {
            mvwaddch(intoWindow, sizeY - 2, i, ' ');
        }
        wattroff(intoWindow, COLOR_PAIR(NCColors::interactive));
        wmove(intoWindow, sizeY - 2, 1);
    }

    wrefresh(intoWindow);
    needsRefresh = false;
}

SettingsView * SettingsView::clone() const {
    return new SettingsView(*this);
}
