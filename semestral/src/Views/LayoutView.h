/****************************************************************
 * @file LayoutView.h
 * @author Michal Dobes
 * @brief Layout view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <memory>

#include "Views/View.h"

/**
 * @brief Layout view
 *
 * Divides screen into left and right portion and creates two window.
 * Primary with size priority and secondary with fixed size.
 *
 */
class LayoutView : public View {
protected:
    WINDOW * primaryWindow; //< Primary window with size priority
    std::unique_ptr<View> primaryView; //< Pointer to view that should be drawn
    // in primaryWindow

    WINDOW * secondaryWindow; //< Secondary window with fixed size
    std::unique_ptr<View> secondaryView; //< Pointer to view that should be drawn
    // in secondaryWindow

    /**
     * @brief Recreate windows with correct sizes
     *
     */
    void recreateWindows();

public:

    /**
     * @brief Construct a new Layout View object
     *
     */
    LayoutView();

    /**
     * @brief Construct a new Layout View object
     *
     * @param toCopy
     */
    LayoutView(const LayoutView & toCopy);

    /**
     * @brief Destroy the Layout View object
     *
     */
    ~LayoutView();

    LayoutView & operator = (const LayoutView & toCopy);

    void draw(WINDOW * intoWindow = stdscr) override;

    void setNeedsRefresh() override;

    /**
     * @brief Deallocate windows
     *
     */
    void removeWindows();

    /**
     * @brief Get primary window
     *
     * @return WINDOW*
     */
    WINDOW * getPrimaryWindow() const;

    /**
     * @brief Get secondary window
     *
     * @return WINDOW*
     */
    WINDOW * getSecondaryWindow() const;

    /**
     * @brief Get pointer to primary view that is drawn in primary window
     *
     * @return View*
     */
    View * getPrimaryView() const;

    /**
     * @brief Get pointer to secondary view that is drawn in secondary window
     *
     * @return View*
     */
    View * getSecondaryView() const;

    /**
     * @brief Set view as primary view
     *
     * @param view primary view
     */
    void setPrimaryView(const View & view);

    /**
     * @brief Set view as secondary view
     *
     * @param view secondary view
     */
    void setSecondaryView(const View & view);

    LayoutView * clone() const override;
};

#endif /* LAYOUTVIEW_H */
