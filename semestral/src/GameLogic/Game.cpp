#include "Game.h"
#include "BoardFileLoader.h"
#include <iostream>

void Game::detectCollisions() {
    Position playerPos = player->getTransform().position;
    Board::Tile::Type playerTile = board->tileAt(playerPos);
    if (Board::Tile::typeAllowsInteraction(playerTile)) {
        if (playerTile == Board::Tile::Type::coin) {
            score += 10;
        } else if (playerTile == Board::Tile::Type::frighten) {
            toggleFrighten(true);
        }

        board->interactWithTileAt(playerPos);
        needsRedraw = true;
    }

    //TODO
}

void Game::movePlayer() {
    needsRedraw = true;
    player->move(*board);
}

void Game::moveEnemy(bool fright) {
    if (frightenActivated >= 1 && !fright) {
        return;
    }

    needsRedraw = true;

    for (auto & e : ghosts) {
        e->move(*board, player->getTransform(), ghosts[0]->getTransform().position);
    }

    if (frightenActivated >= 1) {
        timer.addTrigger(settings.enemySpeed * frightenSpeedMultiplier, [ this ]() {
            this->moveEnemy(true);
            });
    }
}

void Game::createBonus() {
    //TODO
}

void Game::toggleScatter() {
    needsRedraw = true;
    for (auto & e : ghosts) {
        e->toggleScatter();
    }
}

void Game::toggleFrighten(bool on) {
    needsRedraw = true;

    if (on) {
        frightenActivated++;

        timer.addTrigger(settings.frightenDuration, [ this ]() {
            this->toggleFrighten(false);
            });
    } else {
        frightenActivated--;
    }

    if (!(frightenActivated == 0 || (frightenActivated == 1 && on))) {
        return;
    }

    for (auto & e : ghosts) {
        e->toggleFrighten();
    }

    if (on) {
        timer.addTrigger(settings.enemySpeed * frightenSpeedMultiplier, [ this ]() {
            this->moveEnemy(true);
            });
    }
}

Game::Game(const GameSettings & gameSettings, double frightenMultiplier) :
    settings(gameSettings),
    needsRedraw(false),
    board(nullptr),
    player(nullptr),
    score(0),
    bonusOut(false),
    frightenActivated(0),
    frightenSpeedMultiplier(frightenMultiplier) { }

void Game::loadMap(const std::string & filepath) {
    BoardFileLoader fileLoader(filepath);
    board.reset(new Board(fileLoader.loadBoard()));
}

void Game::restart() {
    Transform playerSpawn(board->getPlayerSpawn(), Rotation(Rotation::Direction::left));
    player.reset(new Player(playerSpawn));

    Transform enemySpawn(board->getEnemySpawn(), Rotation(Rotation::Direction::left));
    ghosts[0].reset(new GhostBlinky(enemySpawn, Position(0, board->getSizeX())));
    ghosts[1].reset(new GhostPinky(enemySpawn, Position(0, 0)));
    ghosts[2].reset(new GhostInky(enemySpawn, Position(board->getSizeY(), board->getSizeY())));
    ghosts[3].reset(new GhostClyde(enemySpawn, Position(board->getSizeY(), 0)));

    timer = Timer();

    /* Movement trigger */
    timer.addTrigger(settings.playerSpeed, [ this ]() {
        this->movePlayer();
        }, true);
    timer.addTrigger(settings.enemySpeed, [ this ]() {
        this->moveEnemy();
        }, true);

    /* Chase and scatter trigger */
    /*
    timer.addTrigger(chaseDuration + scatterDuration, [ this ]() {
        this->toggleScatter();
        }, true);
    timer.addTrigger(chaseDuration, [ this ]() {
        this->timer.addTrigger(chaseDuration + scatterDuration, [ this ]() {
            this->toggleScatter();
            }, true);
        }, false);*/

    /* Ghost come out */
    for (size_t i = 0; i < 1; i++) {
        timer.addTrigger(settings.ghostComeOutPeriod * i, [ this, i ]() {
            this->ghosts[i]->toggleAlive();
            });
    }
}

void Game::update(std::optional<Rotation> keyPressDirection) {
    needsRedraw = false;

    if (keyPressDirection) {
        player->rotate(*keyPressDirection);
    }

    if (!isPaused()) {
        timer.update();

        detectCollisions();
    }
}

unsigned int Game::getDimensionX() {
    return board->getSizeX();
}

unsigned int Game::getDimensionY() {
    return board->getSizeY();
}

void Game::togglePause() {
    timer.togglePause();
}

bool Game::isPaused() {
    return timer.isPaused();
}

unsigned long Game::getScore() {
    return score;
}

bool Game::doesNeedRefresh() {
    return needsRedraw;
}