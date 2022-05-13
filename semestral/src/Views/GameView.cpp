#include "GameView.h"
#include <typeinfo>

GameView::GameView() : View(), gameToDraw(nullptr) {
    ableToDisplay = false;
}

GameView::~GameView() { }

void GameView::draw(WINDOW * intoWindow) {

    if (!(needsRefresh || (ableToDisplay && (gameToDraw->needsRedraw)))) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    if (ableToDisplay) {
        for (size_t y = 0; y < gameToDraw->getDimensionY(); y++) {
            for (size_t x = 0; x < gameToDraw->getDimensionX(); x++) {
                DisplayInformation displayTile = Board::Tile::typeDisplay(gameToDraw->board->tileAt(Position(x, y)));
                wattron(intoWindow, COLOR_PAIR(displayTile.second));
                mvwaddch(intoWindow, y, x, displayTile.first);
                wattroff(intoWindow, COLOR_PAIR(displayTile.second));

            }
        }

        Position playerPos = gameToDraw->player->getTransform().position;
        DisplayInformation playerDis = gameToDraw->player->displayEntity();
        wattron(intoWindow, COLOR_PAIR(playerDis.second));
        mvwaddch(intoWindow, playerPos.y, playerPos.x, playerDis.first);
        wattroff(intoWindow, COLOR_PAIR(playerDis.second));


        for (auto & e : gameToDraw->ghosts) {
            if (!e->isAlive()) {
                continue;
            }
            Position enemyPos = e->getTransform().position;
            DisplayInformation enemyDis = e->displayEntity();
            wattron(intoWindow, COLOR_PAIR(enemyDis.second));
            mvwaddch(intoWindow, enemyPos.y, enemyPos.x, enemyDis.first);
            wattroff(intoWindow, COLOR_PAIR(enemyDis.second));
        }

        gameToDraw->needsRedraw = false;
    }

    wrefresh(intoWindow);
    needsRefresh = false;
}

void GameView::setGameToDraw(Game * game) {
    if (game == nullptr) {
        ableToDisplay = false;
    } else {
        ableToDisplay = true;
    }

    setNeedsRefresh();
    gameToDraw = game;
}
