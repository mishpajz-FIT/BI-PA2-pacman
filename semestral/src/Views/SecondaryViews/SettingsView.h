/****************************************************************
 * @file SettingsView.h
 * @author Michal Dobes
 * @brief Settings view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include "Views/SecondaryViews/SecondaryView.h"

/**
 * @brief Settings view
 *
 * Includes option for input field.
 *
 */
class SettingsView : public SecondaryView {
protected:
    void drawHint(WINDOW * intoWindow) override;

    bool inputable; //< Should display input field

public:

    /**
     * @brief Construct a new Settings View object
     *
     * @param input Expects input
     */
    SettingsView(bool input = true);

    /**
     * @brief Destroy the Settings View object
     *
     */
    virtual ~SettingsView();

    void draw(WINDOW * intoWindow = stdscr) override;

    SettingsView * clone() const override;
};

#endif /* SETTINGSVIEW_H */
