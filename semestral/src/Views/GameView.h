/****************************************************************
 * @file GameView.h
 * @author Michal Dobes
 * @brief Game view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
  *****************************************************************/

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <tuple>

#include "Views/View.h"
#include "GameLogic/Game.h"
#include "GameLogic/Board.h"
#include "Utilities/NCColors.h"

/**
 * @brief Game view
 *
 */
class GameView : public View {
protected:
    Game * gameToDraw; //< Pointer to Game to draw

    typedef std::pair<char, NCColors::ColorPairs> DisplayInformation;

    /**
     * @brief Draw char with options into window at position
     *
     * Position is adjusted to be centered in window.
     *
     * @param intoWindow Window to draw char into
     * @param at Position of char
     * @param colorPair Color pair to use when drawing char
     * @param c Char to draw
     */
    void drawGameElement(WINDOW * intoWindow, const Position & at, unsigned int colorPair, char c);

    /**
     * @brief Draw whole game board
     *
     * @param intoWindow Window to draw into
     */
    void drawBoard(WINDOW * intoWindow);

    /**
     * @brief Draw parts of game board that are in game's diffRedraw
     *
     * @param intoWindow Window to draw into
     */
    void drawDiff(WINDOW * intoWindow);

    /**
     * @brief Draw player
     *
     * @param intoWindow Window to draw into
     */
    void drawPlayer(WINDOW * intoWindow);

    /**
     * @brief Draw enemies
     *
     * @param intoWindow Window to draw into
     */
    void drawEnemies(WINDOW * intoWindow);

public:

    /**
     * @brief Construct a new Game View object
     *
     * Note, view should not own the game object.
     * Game object should be owned by other object.
     *
     * @param game pointer to game to draw
     */
    GameView(Game * game);

    /**
     * @brief Destroy the Game View object
     *
     */
    virtual ~GameView();

    void draw(WINDOW * intoWindow = stdscr) override;

    GameView * clone() const override;
};

#endif /* GAMEVIEW_H */