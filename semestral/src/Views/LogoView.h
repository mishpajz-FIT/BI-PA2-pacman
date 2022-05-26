/****************************************************************
 * @file LogoView.h
 * @author Michal Dobes
 * @brief Logo view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef LOGOVIEW_H
#define LOGOVIEW_H

#include "Views/View.h"

// Constants for ascii art
#define LOGOVIEWLOGOWIDTH 44
#define LOGOVIEWLOGOHEIGHT 5 

/**
 * @brief Logo view
 *
 */
class LogoView : public View {
    static const char logo[LOGOVIEWLOGOHEIGHT][LOGOVIEWLOGOWIDTH]; //< Ascii art

public:
    /**
     * @brief Construct a new Logo View object
     *
     */
    LogoView();

    /**
     * @brief Destroy the Logo View object
     *
     */
    virtual ~LogoView();

    void draw(WINDOW * intoWindow = stdscr) override;

    LogoView * clone() const override;
};

#endif /* LOGOVIEW_H */