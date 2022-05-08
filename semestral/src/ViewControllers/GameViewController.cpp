#include "GameViewController.h"
#include "GameFileLoader.h"
#include "BoardFileLoader.h"
#include <string>

GameViewController::GameViewController() : ViewController(), phase(optionsLoading), prevPhase(none) {
    createWindows();

    draw();
    refresh();
    wrefresh(optionWindow);
    wrefresh(gameWindow);
}

void GameViewController::createWindows() {
    gameWindow = newwin(termYSize, termXSize - 50 - 1, 0, 0);
    boxAronudWindow(gameWindow);
    optionWindow = newwin(termYSize, 50, 0, termXSize - 50);
    boxAronudWindow(optionWindow);
}

void GameViewController::update() {
    prevPhase = phase;
    if (!displaying) {
        return;
    }

    problem = false;

    if (phase == optionsLoading) {
        char bufferStr[256];
        wgetnstr(optionWindow, bufferStr, 256);
        std::string expectedPath(bufferStr);
        try {
            GameFileLoader gameLoader(expectedPath);
            game.reset(new Game(gameLoader.loadGame()));
            phase = mapLoading;
        }
        catch (FileLoaderException & e) {
            problem = true;
            problemMessage = "Couldn't load game options";
        }
        return;
    }

    if (phase == mapLoading) {
        char bufferStr[256];
        wgetnstr(optionWindow, bufferStr, 256);
        std::string expectedPath(bufferStr);
        try {
            BoardFileLoader boardLoader(expectedPath);
            game->board.reset(new Board(boardLoader.loadBoard()));
            phase = playing;
        }
        catch (FileLoaderException & e) {
            problem = true;
            problemMessage = "Couldn't load game map";
        }
        return;
    }

    if (phase == playing) {
        game->update();
    }
}

void GameViewController::drawGame() {
    if (phase == playing && game->needsRedraw) {
        for (size_t y = 0; y < game->board->getSizeY(); y++) {
            for (size_t x = 0; x < game->board->getSizeX(); x++) {
                mvwaddch(gameWindow, y, x, Board::Tile::typeDisplayChar(game->board->tileAt(Position(x, y))));
            }
        }

        Position positionToWork = game->player->getTransform().position;
        mvwaddch(gameWindow, positionToWork.y, positionToWork.x, game->player->displayChar());

        for (auto & e : game->ghosts) {
            positionToWork = e->getTransform().position;
            mvwaddch(gameWindow, positionToWork.y, positionToWork.x, e->displayChar());
        }

        game->needsRedraw = false;
        wrefresh(gameWindow);
    }
}

void GameViewController::drawOptions() {
    if (prevPhase != phase) {
        wclear(optionWindow);
        boxAronudWindow(optionWindow);

        if (phase == optionsLoading || phase == mapLoading) {
            curs_set(2);
            echo();
        } else {
            curs_set(0);
            noecho();
        }
    }

    if (problem) {
        mvwprintw(optionWindow, termYSize - 2, 2, problemMessage.c_str());
    } else if (!problem && prevProblem) {
        wclear(optionWindow);
        boxAronudWindow(optionWindow);
    }

    std::string title;
    switch (phase) {
        case optionsLoading:
            title = "Enter path to options file";
            break;
        case mapLoading:
            title = "Enter path to map file";
            break;
        default:
            break;
    }
    mvwprintw(optionWindow, 1, 1, title.c_str());

    wmove(optionWindow, 10, 2);
    wrefresh(optionWindow);
}


void GameViewController::draw() {
    getTermSizes();

    if (termSizeChanged) {
        int minXsize = 50;
        int minYsize = 20;
        if (phase == playing) {
            minXsize += game->board->getSizeX();
            minYsize = (game->board->getSizeY() > 50 ? game->board->getSizeY() : 50);
        }

        if (minXsize > termXSize || minYsize > termYSize) {
            clear();
            wclear(optionWindow);
            wclear(gameWindow);
            wrongResolution();
            refresh();
            wrefresh(optionWindow);
            wrefresh(gameWindow);
            return;
        } else {
            clear();
            delwin(gameWindow);
            delwin(optionWindow);
            createWindows();
            refresh();
            wrefresh(gameWindow);
            wrefresh(optionWindow);
        }
    }

    drawOptions();
    drawGame();
}