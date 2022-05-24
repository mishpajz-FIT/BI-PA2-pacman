#include <typeinfo>

#include "Views/GameView.h"

void GameView::drawGameElement(WINDOW * intoWindow, const Position & at, unsigned int colorPair, char c) {
    wattron(intoWindow, COLOR_PAIR(colorPair));
    mvwaddch(intoWindow, centeredYInWindow(at.y), centeredXInWindow(at.x), c);
    wattroff(intoWindow, COLOR_PAIR(colorPair));
}

void GameView::drawBoard(WINDOW * intoWindow) {
    for (size_t y = 0; y < gameToDraw->getDimensionY(); y++) {
        for (size_t x = 0; x < gameToDraw->getDimensionX(); x++) {
            Position pos(x, y);
            DisplayInformation displayTile = Board::Tile::typeDisplay(gameToDraw->board->tileAt(pos));
            drawGameElement(intoWindow, pos, displayTile.second, displayTile.first);
        }
    }
}

void GameView::drawPlayer(WINDOW * intoWindow) {
    DisplayInformation displayPlayer = gameToDraw->player->displayEntity();
    drawGameElement(intoWindow, gameToDraw->player->getTransform().position, displayPlayer.second, displayPlayer.first);
}

void GameView::drawEnemies(WINDOW * intoWindow) {
    for (auto & e : gameToDraw->ghosts) {
        if (!e->isAlive()) {
            continue;
        }
        DisplayInformation displayEnemy = e->displayEntity();
        drawGameElement(intoWindow, e->getTransform().position, displayEnemy.second, displayEnemy.first);
    }
}

GameView::GameView(Game * game) : View(), gameToDraw(game) {
    ableToDisplay = false;

    if (gameToDraw != nullptr) {
        ableToDisplay = true;
        minSizeX = gameToDraw->getDimensionX() + 1;
        minSizeY = gameToDraw->getDimensionY() + 1;
    }
}

GameView::~GameView() { }

void GameView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!(needsRefresh || (ableToDisplay && ((gameToDraw->doesNeedRefresh()) || gameToDraw->isPaused())))) {
        return;
    }

    if (sizeChanged) {
        wclear(intoWindow);
        box(intoWindow, 0, 0);
    }

    if (isAbleToDisplay()) {
        drawBoard(intoWindow);
        drawPlayer(intoWindow);
        drawEnemies(intoWindow);
    }

    wnoutrefresh(intoWindow);
    needsRefresh = false;
}

GameView * GameView::clone() const {
    return new GameView(*this);
}