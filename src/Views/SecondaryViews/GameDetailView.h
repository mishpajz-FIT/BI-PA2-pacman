/****************************************************************
 * @file GameDetailView.h
 * @author Michal Dobes
 * @brief Game detail view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAMEDETAILVIEW_H
#define GAMEDETAILVIEW_H

#include "Views/SecondaryViews/SecondaryView.h"
#include "GameLogic/Game.h"

/**
 * @brief Game detail view
 *
 */
class GameDetailView : public SecondaryView {
protected:
    Game * gameToDraw; //< Pointer to Game to draw

    void drawHint(WINDOW * intoWindow) override;

public:
    /**
     * @brief Construct a new Game Detail View object
     *
     * Note, view should not own the game object.
     * Game object should be owned by other object.
     *
     * @param game pointer to game to draw
     */
    GameDetailView(Game * game);

    /**
     * @brief Destroy the Game Detail View object
     *
     */
    virtual ~GameDetailView();

    void draw(WINDOW * intoWindow = stdscr) override;

    GameDetailView * clone() const override;
};

#endif /* GAMEDETAILVIEW_H */