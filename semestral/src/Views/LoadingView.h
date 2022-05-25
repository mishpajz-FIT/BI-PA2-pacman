/****************************************************************
 * @file LoadingView.h
 * @author Michal Dobes
 * @brief Loading view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef LOADINGVIEW_H
#define LOADINGVIEW_H

#include "Views/View.h"

// Constants for ascii art
#define LOADINGVIEWFRAMES 5 
#define LOADINGVIEWFRAMEHEIGHT 12
#define LOADINGVIEWFRAMEWIDTH 32

/**
 * @brief Loading view
 *
 * Animates ascii pacman, on each draw call switches to different frame.
 *
 */
class LoadingView : public View {

    // Frames of ascii art
    static const char frames[LOADINGVIEWFRAMES][LOADINGVIEWFRAMEHEIGHT][LOADINGVIEWFRAMEWIDTH + 1];
    unsigned int currentFrame; //< Current frame of ascii art
    bool countingUp; //< Direction of counting

    /**
     * @brief Switch to next frame
     *
     * Changes frame up or down based on countingUp, or if at bound toggle countingUp
     *
     */
    void nextFrame();

public:
    /**
     * @brief Construct a new Loading View object
     *
     */
    LoadingView();

    /**
     * @brief Destroy the Loading View object
     *
     */
    virtual ~LoadingView();

    void draw(WINDOW * intoWindow = stdscr) override;

    LoadingView * clone() const override;
};

#endif /* LOADINGVIEW_H */