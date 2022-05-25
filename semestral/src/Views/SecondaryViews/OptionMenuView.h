/****************************************************************
 * @file OptionMenuView.h
 * @author Michal Dobes
 * @brief Option menu view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <string>
#include <vector>

#include "Views/SecondaryViews/SecondaryView.h"
#include "Utilities/Contexts/OptionMenu.h"

/**
 * @brief Option menu view
 *
 */
class OptionMenuView : public SecondaryView {
protected:
    OptionMenu * menuToDraw; //< Pointer to OptionMenu to draw

    void drawHint(WINDOW * intoWindow) override;

public:
    /**
     * @brief Construct a new Option Menu View object
     * 
     * Note, view should not own the game object.
     * OptionMenu object should be owned by other object.
     * 
     * @param menu pointer to option menu to draw
     */
    OptionMenuView(OptionMenu * menu);

    /**
     * @brief Destroy the Option Menu View object
     * 
     */
    virtual ~OptionMenuView();

    void draw(WINDOW * intoWindow = stdscr) override;

    OptionMenuView * clone() const override;
};

#endif /* MENUVIEW_H */