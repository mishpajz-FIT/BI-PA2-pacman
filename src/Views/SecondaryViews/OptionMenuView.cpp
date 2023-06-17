#include "Views/SecondaryViews/OptionMenuView.h"
#include "Utilities/NCColors.h"

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
    } else {
        if (menuToDraw->size() >= 15) {
            minSizeY = 11 + menuToDraw->size();
        }
    }
}
OptionMenuView::~OptionMenuView() { }

void OptionMenuView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!(needsRefresh || (isAbleToDisplay() && menuToDraw->needsRefresh))) {
        return;
    }


    // Update minimal size by option menu size
    if (menuToDraw->options.size() >= 15) {
        minSizeY = 11 + menuToDraw->size();
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    wattron(intoWindow, A_BOLD);
    mvwprintw(intoWindow, 1, centeredXForText(titleText), titleText.c_str());
    wattroff(intoWindow, A_BOLD);

    if (isAbleToDisplay()) {

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

    wnoutrefresh(intoWindow);
    needsRefresh = false;
}

OptionMenuView * OptionMenuView::clone() const {
    return new OptionMenuView(*this);
}