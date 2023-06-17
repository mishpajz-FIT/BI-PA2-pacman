/****************************************************************
 * @file OptionMenu.h
 * @author Michal Dobes
 * @brief Option menu model
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <vector>
#include <string>
#include <optional>

/**
 * @brief Option menu
 *
 * Stores options and current selection.
 *
 */
class OptionMenu {
    // View as friend class so it have access to internal values needed for
    // effective displaying
    friend class OptionMenuView;

private:
    bool needsRefresh; //< Indicator if values that can be displayed have changed
    std::vector<std::string> options; //< Option names
    unsigned int currentOption; //< Current selected option

public:

    /**
     * @brief Construct a new Option Menu object
     *
     */
    OptionMenu();

    /**
     * @brief Destroy the Option Menu object
     *
     */
    ~OptionMenu();

    /**
     * @brief Add new option
     *
     * @param name Name of option
     */
    void addOption(const std::string & name);

    /**
     * @brief Change current selection by one
     *
     * Guards crossing of bounds.
     *
     * @param up increase current selection, else decreases
     */
    void changeSelection(bool up = true);

    /**
     * @brief Get index current selection
     *
     * @return unsigned int
     */
    unsigned int getCurrentOption() const;

    /**
     * @brief Get name of current selection
     *
     * @return std::string
     */
    std::string getCurrentOptionName() const;

    /**
     * @brief Handle input from keyboard
     *
     * Tries to perform action based on input
     *
     * Arrow UP - changes selection by one less
     * Arrow DOWN - changes selection by one more
     * ENTER - confirms current selection
     *
     * @param c value of key
     * @return std::optional<unsigned int> If not empty current selection was confirmed,
     *      contains index of the selection, else selection was changed or no action was taken
     *
     */
    std::optional<unsigned int> handleInput(int c);

    /**
     * @brief Get number of options
     *
     * @return unsigned int
     */
    unsigned int size() const;
};

#endif /* OPTIONMENU_H */