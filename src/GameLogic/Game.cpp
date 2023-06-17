#include <iostream>

#include "GameLogic/Game.h"
#include "Utilities/FileManagers/BoardFileLoader.h"

void Game::detectCollisions() {
    Position playerPos = player->getTransform().position;
    Board::Tile::Type playerTile = board->tileAt(playerPos);

    // Check for collision of player and interactable tile
    if (Board::Tile::typeAllowsInteraction(playerTile)) {
        // Perform different action based on tile
        switch (playerTile) {
            case Board::Tile::Type::coin:
                score += 10;
                break;
            case Board::Tile::Type::frighten:
                toggleFrighten(true);
                break;
            case Board::Tile::Type::bonus:
                score += 100;
                break;
            default:
                break;
        }

        board->interactWithTileAt(playerPos); //< Replace tile in board after interaction
        needsRedraw = true;
    }

    // Check for collision of player and enemy
    for (auto & e : ghosts) {
        if (!e->isAlive()) {
            continue;
        }

        if (playerPos == e->getTransform().position) {
            if (e->isFrightened()) { //< If enemy is frightned, kill enemy and reset it
                diffRedraw.push_back(e->getTransform().position); //< Add previous position 
                 // of enemy into diffRedraw
                e->toggleAlive();
                if (e->isFrightened()) {
                    e->toggleFrighten(*board);
                }
                e->reposition(board->getEnemySpawn());

                // Add timer trigger for enemy respawn
                Enemy * ePtr = e.get();
                timer.addTrigger(
                    settings.killDuration,
                    [ ePtr ]() {
                        ePtr->toggleAlive();
                    });

                score += (200 * (killStreak + 1)); //< Raise score by multiplied by Killstreak
                killStreak++;

                needsRedraw = true;
            } else { //< If enemy is not frightened, restart game and substract lives
                if (!timer.isPaused()) {
                    togglePause();
                }

                // Add positions of all entities into diffRedraw
                diffRedraw.push_back(playerPos);
                for (auto & e : ghosts) {
                    diffRedraw.push_back(e->getTransform().position);
                }

                restart();
                lives--;
                break;
            }
        }
    }
}

void Game::movePlayer() {
    needsRedraw = true;
    diffRedraw.push_back(player->getTransform().position); //< Add previous position
    // of player into diffRedraw
    player->move(*board);
}

void Game::moveEnemy(bool fright) {
    needsRedraw = true;

    for (auto & e : ghosts) {
        diffRedraw.push_back(e->getTransform().position); //< Add previous enemy position
        // into diffRedraw

        // If method fright mode matches enemy's fright mode move enemy
        if ((e->isFrightened() && fright) || (!e->isFrightened() && !fright)) {
            // Pass Blinky's position as special position for movement target calculation
            e->move(*board, player->getTransform(), ghosts[0]->getTransform().position);
        }
    }

    // Add next fright move timer trigger if move is in fright mode and frightened mode 
    // wasn't switched off yet
    if (frightenActivated >= 1 && fright) {
        // Timer trigger can't be cerated as repeated, because it wouldn't be possible to turn off 
        timer.addTrigger(settings.enemySpeed * frightenSpeedMultiplier, [ this ]() {
            this->moveEnemy(true);
            });
    }
}

void Game::createBonus() {
    needsRedraw = true;
    std::optional<Position> bonusPos = board->placeBonusTile();
    if (bonusPos) {
        // Add position of replaced tile into diffRedraw
        diffRedraw.push_back(*bonusPos);
    }
}

void Game::toggleScatter() {
    needsRedraw = true;
    for (auto & e : ghosts) {
        e->toggleScatter(*board);
    }
}

void Game::toggleFrighten(bool on) {
    needsRedraw = true;

    // On on, raise frightenActivated count and create timer trigger to turn frighten
    // back off after some time
    // On off, decrease frightenActivated count and reset killStreak
    if (on) {
        frightenActivated++;

        timer.addTrigger(settings.frightenDuration, [ this ]() {
            this->toggleFrighten(false);
            });
    } else {
        frightenActivated--;
        killStreak = 0;
    }

    // If enemy is alive and
    // If toggle is on and enemy is not in frightened mode, toggle frightened on enemy
    // If toggle is off and is last active frighten (no other frighten is activated),
    // toggle frightened off all enemies that are frightened
    for (auto & e : ghosts) {
        if (!e->isAlive()) {
            continue;
        }

        if ((!e->isFrightened() && on)
            || (e->isFrightened() && frightenActivated == 0)) {
            e->toggleFrighten(*board);
        }
    }

    // If only this frighten is activated, create timer trigger for frighten movement
    if (on && frightenActivated == 1) {
        timer.addTrigger(settings.enemySpeed * frightenSpeedMultiplier, [ this ]() {
            this->moveEnemy(true);
            });
    }
}

Game::Game(
    const GameSettings & gameSettings,
    double frightenMultiplier,
    unsigned int livesAmount,
    unsigned int enemyLevel)
    :
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

void Game::loadBoard(const Board & map) {
    board.reset(new Board(map));
}

void Game::restart() {
    // Resets timer, killStreak, frightenActivated.
    // Reallocates all entities and sets them to its initial transforms.
    // Creates timer triggers for actions.

    if (board.get() == nullptr) {
        return;
    }

    timer = Timer();

    killStreak = 0;
    frightenActivated = 0;

    // Create new player entity
    Transform playerSpawn(board->getPlayerSpawn(), Rotation(Rotation::Direction::left));
    player.reset(new Player(playerSpawn));

    // Create four new ghosts
    Transform enemySpawn(board->getEnemySpawn(), Rotation(Rotation::Direction::left));
    ghosts.resize(4);
    ghosts[0].reset(
        new GhostBlinky(enemySpawn, Position(0, board->getSizeX()), enemyIntelligence));
    ghosts[1].reset(
        new GhostPinky(enemySpawn, Position(0, 0), enemyIntelligence));
    ghosts[2].reset(
        new GhostInky(enemySpawn, Position(board->getSizeY(), board->getSizeY()), enemyIntelligence));
    ghosts[3].reset(
        new GhostClyde(enemySpawn, Position(board->getSizeY(), 0), enemyIntelligence));

    // Create movement timer triggers
    timer.addTrigger(settings.playerSpeed, [ this ]() {
        this->movePlayer();
        }, true);
    timer.addTrigger(settings.enemySpeed, [ this ]() {
        this->moveEnemy();
        }, true);

    // Create bonus creation timer triggers
    timer.addTrigger(settings.bonusPeriod, [ this ]() {
        this->createBonus();
        }, true);

    // Create chase and scatter modes timer triggers
    timer.addTrigger(settings.chaseDuration + settings.scatterDuration, [ this ]() {
        this->toggleScatter();
        }, true);
    timer.addTrigger(settings.scatterDuration, [ this ]() {
        this->timer.addTrigger(settings.chaseDuration + settings.scatterDuration, [ this ]() {
            this->toggleScatter();
            }, true);
        }, false);

    // Create triggers for ghosts to come out
    for (size_t i = 0; i < ghosts.size(); i++) {
        timer.addTrigger(settings.ghostComeOutPeriod * i, [ this, i ]() {
            this->ghosts[i]->toggleAlive();
            });
    }

    needsRedraw = true;
}

void Game::update(std::optional<Rotation> keyPressDirection) {
    // Reset variables indicating changes that should be displayed
    needsRedraw = false;
    diffRedraw.clear();

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
    needsRedraw = true;
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

unsigned int Game::getCoinsRemaining() {
    return board->getNumberOfCoins();
}

bool Game::doesNeedRefresh() {
    return needsRedraw;
}