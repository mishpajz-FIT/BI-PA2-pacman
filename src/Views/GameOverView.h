/****************************************************************
 * @file GameOverView.h
 * @author Michal DobeS
 * @brief Game over view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAMEOVERVIEW_H
#define GAMEOVERVIEW_H

#include "Views/View.h"

/**
 * @brief Game over view
 *
 */
class GameOverView : public View {
private:
    unsigned long score; //< Achieved score
    unsigned long highscore; //< Highscore for this configuration
    bool isHighscore; //< Reached highscore

public:

    /**
     * @brief Construct a new Game Over View object
     *
     * @param reachedScore Achieved score
     * @param previousHighscore Highscore for this configuration
     * @param isRecord Did reach highscore
     */
    GameOverView(unsigned long reachedScore, unsigned long previousHighscore, bool isRecord);

    /**
     * @brief Destroy the Game Over View object
     *
     */
    ~GameOverView();

    void draw(WINDOW * intoWindow = stdscr) override;

    GameOverView * clone() const override;
};

#endif /* GAMEOVERVIEW_H */