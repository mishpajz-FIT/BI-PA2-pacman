/****************************************************************
 * @file View.h
 * @author Michal Dobes
 * @brief View
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include <stdexcept>
#include <string>

/**
 * @brief View
 *
 * Used for displaying information to the screen.
 * Data shoud be held separately from Views.
 *
 */
class View {
protected:
    bool needsRefresh; //< Indicator that whole view needs to be redrawn

    bool ableToDisplay; //< Indicator that the view has problem with displaying

    unsigned int sizeY; //< Size y of view
    unsigned int sizeX; //< Size x of view
    bool sizeChanged; //< Size has changed since last update
    unsigned int minSizeY; //< Minimum size y of view
    unsigned int minSizeX; //< Minimum size x of view

    bool warningDisplayed; //< Should display warning
    std::string warningText; //< Text of warning

    std::string titleText; //< Text of title

    /**
     * @brief Get size of window and update variables assocatied with it
     *
     * @param forWindow Window to get size of
     */
    void getWindowSize(WINDOW * forWindow = stdscr);

    /**
     * @brief Adjust x coordinate to be centered in view
     *
     * @param x coordinate
     * @return unsigned int
     */
    unsigned int centeredXInWindow(unsigned int x) const;

    /**
     * @brief Adjust y coordinate to be centered in view
     *
     * @param y coordinate
     * @return unsigned int
     */
    unsigned int centeredYInWindow(unsigned int y) const;

    /**
     * @brief Get x coordiante so the text is centered in view
     *
     * @param text text to center
     * @return unsigned int
     */
    unsigned int centeredXForText(const std::string & text) const;

public:
    /**
     * @brief Construct a new View object
     *
     */
    View();

    /**
     * @brief Destroy the View object
     *
     */
    virtual ~View();

    /**
     * @brief Turns on indication that whole view (and subviews) should be redrawn
     *
     */
    virtual void setNeedsRefresh();

    /**
     * @brief Is view able to draw data on screen
     *
     * @return true
     * @return false
     */
    bool isAbleToDisplay();

    /**
     * @brief Draw into window
     *
     * @param intoWindow window to draw into
     */
    virtual void draw(WINDOW * intoWindow = stdscr) = 0;

    /**
     * @brief Allocate copy of this
     *
     * @return View* pointer to copy of this
     */
    virtual View * clone() const = 0;

    /**
     * @brief Toggle warning and set warning text
     *
     * Also calls setNeedsRefresh
     *
     * @param to toggle warning on/off
     * @param text text of warning
     */
    void setWarning(bool to, std::string text = "");

    /**
     * @brief Set view's title
     *
     * @param text text of title
     */
    void setTitle(std::string text);

    /**
     * @brief Get minimum size in x dimension
     *
     * @return unsigned int
     */
    unsigned int getMinSizeX();

    /**
     * @brief Get minimum size in y dimension
     *
     * @return unsigned int
     */
    unsigned int getMinSizeY();
};

/**
 * @brief View Exception
 *
 */
struct ViewException : public std::runtime_error {
    ViewException(const std::string & message);
};

#endif /* VIEW_H */