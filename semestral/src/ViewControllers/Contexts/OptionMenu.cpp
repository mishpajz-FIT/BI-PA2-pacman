#include "OptionMenu.h"

OptionMenu::OptionMenu() : currentOption(0) { }

OptionMenu::~OptionMenu() { }

void OptionMenu::addOption(const std::string & name) {
    options.push_back(name);
}

void OptionMenu::changeSelection(bool up) {
    if (up && currentOption + 1 < options.size()) {
        currentOption++;
    } else if (!up && currentOption > 0) {
        currentOption--;
    }
}

unsigned int OptionMenu::getCurrentOption() const {
    return currentOption;
}