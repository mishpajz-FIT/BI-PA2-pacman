#include "OptionMenuView.h"
#include "NCColors.h"

void OptionMenuView::drawHint(WINDOW * intoWindow) {
    wattron(intoWindow, COLOR_PAIR(NCColors::ColorPairs::hint));
    mvwprintw(intoWindow, sizeY - 6, 1, "arrow up, down - selection");
    mvwprintw(intoWindow, sizeY - 5, 1, "enter - confirmation");
    mvwprintw(intoWindow, sizeY - 4, 1, "q - quit");
    wattroff(intoWindow, COLOR_PAIR(NCColors::ColorPairs::hint));
}

OptionMenuView::OptionMenuView(OptionMenu * menu) : SecondaryView(), menuToDraw(menu) {
    if (menuToDraw == nullptr) {
        ableToDisplay = false;
    }
}
OptionMenuView::~OptionMenuView() { }

void OptionMenuView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!(needsRefresh || (isAbleToDisplay() && menuToDraw->needsRefresh))) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    wattron(intoWindow, A_BOLD);
    mvwprintw(intoWindow, 1, centeredXForText(titleText), titleText.c_str());
    wattroff(intoWindow, A_BOLD);

    if (isAbleToDisplay()) {
        if (menuToDraw->options.size() >= 15) {
            minSizeX = 11 + menuToDraw->options.size();
        }

        for (size_t i = 0; i < menuToDraw->options.size(); i++) {
            if (i == menuToDraw->getCurrentOption()) {
                wattron(intoWindow, COLOR_PAIR(NCColors::ColorPairs::interactive));
            }
            mvwprintw(intoWindow, 3 + i, 1, menuToDraw->options[i].c_str());
            if (i == menuToDraw->getCurrentOption()) {
                wattroff(intoWindow, COLOR_PAIR(NCColors::ColorPairs::interactive));
            }
        }
    }

    if (warningDisplayed) {
        wattron(intoWindow, A_DIM);
        mvwprintw(intoWindow, sizeY - 2, 1, warningText.c_str());
        wattroff(intoWindow, A_DIM);
    }

    drawHint(intoWindow);

    wrefresh(intoWindow);
    needsRefresh = false;
}

OptionMenuView * OptionMenuView::clone() const {
    return new OptionMenuView(*this);
}