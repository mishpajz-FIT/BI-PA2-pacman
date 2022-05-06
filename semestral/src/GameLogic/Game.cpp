#include "Game.h"
#include "BoardFileLoader.h"
#include <iostream>

void Game::detectCollisions() {
    //TODO
}

void Game::movePlayer() {
    player->move(*board);
    std::cout << "(player) " << player->getTransform().position.x << "; " << player->getTransform().position.y << std::endl;
}

void Game::moveEnemy() {
    for (auto & e : ghosts) {
        e->move(*board, player->getTransform(), ghosts[0]->getTransform().position);
        std::cout << "(enemy) " << e->getTransform().position.x << "; " << e->getTransform().position.y << std::endl;
    }
}

void Game::createBonus() {
    //TODO
}

Game::Game(
    unsigned int playerSpd,
    unsigned int enemySpd,
    unsigned int scatterDur,
    unsigned int chaseDur,
    unsigned int frightenDur,
    unsigned int killDur,
    unsigned int bonusPer,
    unsigned int ghostComeOutPer
) :
    board(nullptr),
    player(nullptr),
    score(0),
    bonusOut(false),
    paused(true),
    playerSpeed(playerSpd),
    enemySpeed(enemySpd),
    scatterDuration(scatterDur),
    chaseDuration(chaseDur),
    frightenDuration(frightenDur),
    killDuration(killDur),
    bonusPeriod(bonusPer),
    ghostComeOutPeriod(ghostComeOutPer) { }

void Game::loadMap(const std::string & filepath) {
    BoardFileLoader fileLoader(filepath);
    board = std::make_unique<Board>(fileLoader.loadBoard());

    Transform playerSpawn(board->getPlayerSpawn(), Rotation(Rotation::Direction::up));
    player = std::make_unique<Player>(Player(playerSpawn));

    Transform enemySpawn(board->getEnemySpawn(), Rotation(Rotation::Direction::left));
    ghosts[0] = std::move(std::make_unique<Enemy>(GhostBlinky(enemySpawn, Position(0, board->getSizeX()))));
    ghosts[1] = std::move(std::make_unique<Enemy>(GhostPinky(enemySpawn, Position(0, 0))));
    ghosts[2] = std::move(std::make_unique<Enemy>(GhostInky(enemySpawn, Position(board->getSizeY(), board->getSizeY()))));
    ghosts[3] = std::move(std::make_unique<Enemy>(GhostClyde(enemySpawn, Position(board->getSizeY(), 0))));

    /* Movement trigger */
    timer.addTrigger(playerSpeed, [ this ]() {
        this->movePlayer();
        }, true);
    timer.addTrigger(enemySpeed, [ this ]() {
        this->moveEnemy();
        }, true);

    /* Chase and scatter trigger */
    timer.addTrigger(chaseDuration + scatterDuration, [ this ]() {
        this->toggleScatter();
        }, true);
    timer.addTrigger(chaseDuration, [ this ]() {
        this->timer.addTrigger(chaseDuration + scatterDuration, [ this ]() {
            this->toggleScatter();
            }, true);
        }, false);

    /* Ghost come out */
    for (size_t i = 0; i < 4; i++) {
        timer.addTrigger(ghostComeOutPeriod * i, [ this, i ]() {
            this->ghosts[i]->toggleAlive();
            });
    }
}

void Game::update() {
    timer.update();
}