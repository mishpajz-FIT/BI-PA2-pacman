/****************************************************************
 * @file MainMenuViewController.h
 * @author Michal Dobes
 * @brief Main menu view controller
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef MAINMENUVIEWCONTROLLER_H
#define MAINMENUVIEWCONTROLLER_H

#include <memory>

#include "ViewControllers/ViewController.h"
#include "Views/LayoutView.h"
#include "Utilities/Contexts/OptionMenu.h"

/**
 * @brief Main menu view controller
 *
 * Controls main menu.
 *
 */
class MainMenuViewController : public ViewController {

    LayoutView layoutView; //< Main layout view

    std::unique_ptr<OptionMenu> menu; //< Menu object

    /**
     * @brief Handle selected value in menu
     *
     * @param i index of selected value
     */
    void handleMenuSelect(unsigned int i);

public:

    /**
     * @brief Construct a new Main Menu View Controller object
     *
     */
    MainMenuViewController();

    /**
     * @brief Destroy the Main Menu View Controller object
     *
     */
    ~MainMenuViewController();

    AppState update() override;

    void draw() override;

};

#endif /* MAINMENUVIEWCONTROLLER_H */