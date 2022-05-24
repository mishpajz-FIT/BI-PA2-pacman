#include <ncurses.h>

#include "OptionMenu.h"

OptionMenu::OptionMenu() : needsRefresh(true), currentOption(0) { }

OptionMenu::~OptionMenu() { }

void OptionMenu::addOption(const std::string & name) {
    options.push_back(name);
}

void OptionMenu::changeSelection(bool up) {
    if (up && currentOption + 1 < size()) {
        currentOption++;
    } else if (!up && currentOption > 0) {
        currentOption--;
    }
}

unsigned int OptionMenu::getCurrentOption() const {
    return currentOption;
}

std::string OptionMenu::getCurrentOptionName() const {
    return options[currentOption];
}


std::optional<unsigned int> OptionMenu::handleInput(int c) {
    needsRefresh = false;
    switch (c) {
        case KEY_UP:
            changeSelection(false);
            needsRefresh = true;
            return { };
        case KEY_DOWN:
            changeSelection(true);
            needsRefresh = true;
            return { };
        case '\n':
            if (currentOption >= size()) {
                return { };
            }
            return getCurrentOption();
        default:
            break;
    }
    return { };
}

unsigned int OptionMenu::size() const {
    return options.size();
}