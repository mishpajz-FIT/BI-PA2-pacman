#include "Views/GameOverView.h"

GameOverView::GameOverView(unsigned long reachedScore, unsigned long previousHighscore, bool isRecord) :
    score(reachedScore),
    highscore(previousHighscore),
    isHighscore(isRecord) {
    minSizeY = 10;
    minSizeX = 2;
}

GameOverView::~GameOverView() { }

void GameOverView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    std::string displayText;

    // Display different layout if highscore or not
    if (isHighscore) {
        displayText = "Highscore!";

        wattron(intoWindow, A_BLINK);
        wattron(intoWindow, A_BOLD);
        mvwprintw(intoWindow, 5, centeredXForText(displayText), displayText.c_str());
        wattroff(intoWindow, A_BOLD);
        wattroff(intoWindow, A_BLINK);
    } else {
        displayText = "highscore: ";
        displayText += std::to_string(highscore);

        mvwprintw(intoWindow, 7, centeredXForText(displayText), displayText.c_str());

        if (displayText.length() + 2 > minSizeX) {
            minSizeX = displayText.length() + 2;
        }
    }

    displayText = std::to_string(score);
    wattron(intoWindow, A_BOLD);
    mvwprintw(intoWindow, 6, centeredXForText(displayText), displayText.c_str());
    wattroff(intoWindow, A_BOLD);

    wnoutrefresh(intoWindow);
    needsRefresh = false;
}

GameOverView * GameOverView::clone() const {
    return new GameOverView(*this);
}