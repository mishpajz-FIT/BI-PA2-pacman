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

    for (auto & e : ghosts) {
        if (!e->isAlive()) {
            continue;
        }

        if (playerPos == e->getTransform().position) {
            if (e->isFrightened()) {
                e->toggleAlive();
                if (e->isFrightened()) {
                    e->toggleFrighten();
                }
                e->reposition(board->getEnemySpawn());
                Enemy * ePtr = e.get();
                timer.addTrigger(
                    settings.killDuration,
                    [ ePtr ]() {
                        ePtr->toggleAlive();
                    });

                score += (200 * (killStreak + 1));
                killStreak++;

                needsRedraw = true;
            } else {
                if (!timer.isPaused()) {
                    togglePause();
                }
                restart();
                lives--;
            }
        }
    }
}

void Game::movePlayer() {
    needsRedraw = true;
    player->move(*board);
}

void Game::moveEnemy(bool fright) {
    needsRedraw = true;

    for (auto & e : ghosts) {
        if ((e->isFrightened() && fright) || (!e->isFrightened() && !fright)) {
            e->move(*board, player->getTransform(), ghosts[0]->getTransform().position);
        }
    }

    if (frightenActivated >= 1 && fright) {
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
        killStreak = 0;
    }

    for (auto & e : ghosts) {
        if (!e->isAlive()) {
            continue;
        }

        if ((!e->isFrightened() && on)
            || (e->isFrightened() && frightenActivated == 0)) {
            e->toggleFrighten();
        }
    }

    if (on && frightenActivated == 1) {
        timer.addTrigger(settings.enemySpeed * frightenSpeedMultiplier, [ this ]() {
            this->moveEnemy(true);
            });
    }
}

Game::Game(const GameSettings & gameSettings, double frightenMultiplier, unsigned int livesAmount, unsigned int enemyLevel) :
    settings(gameSettings),
    needsRedraw(false),
    board(nullptr),
    player(nullptr),
    enemyIntelligence(enemyLevel),
    score(0),
    lives(livesAmount),
    killStreak(0),
    frightenActivated(0),
    frightenSpeedMultiplier(frightenMultiplier) { }

void Game::loadMap(const std::string & filepath) {
    BoardFileLoader fileLoader(filepath);
    board.reset(new Board(fileLoader.loadBoard()));
}

void Game::restart() {
    killStreak = 0;
    frightenActivated = 0;

    Transform playerSpawn(board->getPlayerSpawn(), Rotation(Rotation::Direction::left));
    player.reset(new Player(playerSpawn));

    Transform enemySpawn(board->getEnemySpawn(), Rotation(Rotation::Direction::left));
    ghosts[0].reset(new GhostBlinky(enemySpawn, Position(0, board->getSizeX()), enemyIntelligence));
    ghosts[1].reset(new GhostPinky(enemySpawn, Position(0, 0), enemyIntelligence));
    ghosts[2].reset(new GhostInky(enemySpawn, Position(board->getSizeY(), board->getSizeY()), enemyIntelligence));
    ghosts[3].reset(new GhostClyde(enemySpawn, Position(board->getSizeY(), 0), enemyIntelligence));

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

    needsRedraw = true;
}

void Game::update(std::optional<Rotation> keyPressDirection) {
    needsRedraw = false;

    if (keyPressDirection) {
        player->rotate(*keyPressDirection);

        if (isPaused()) {
            togglePause();
        }
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

unsigned int Game::getLives() {
    return lives;
}

bool Game::doesNeedRefresh() {
    return needsRedraw;
}