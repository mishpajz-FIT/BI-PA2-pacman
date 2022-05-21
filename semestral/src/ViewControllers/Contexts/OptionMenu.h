#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <vector>
#include <string>

class OptionMenu {
    friend class OptionMenuView;

protected:
    std::vector<std::string> options;
    unsigned int currentOption;

public:
    OptionMenu();
    ~OptionMenu();

    void addOption(const std::string & name);

    void changeSelection(bool up);

    unsigned int getCurrentOption() const;
};

#endif /* OPTIONMENU_H */