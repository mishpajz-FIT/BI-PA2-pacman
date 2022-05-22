#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <vector>
#include <string>
#include <optional>

class OptionMenu {
    friend class OptionMenuView;

protected:
    bool needsRefresh;
    std::vector<std::string> options;
    unsigned int currentOption;

public:
    OptionMenu();
    ~OptionMenu();

    void addOption(const std::string & name);

    void changeSelection(bool up);

    unsigned int getCurrentOption() const;

    std::string getCurrentOptionName() const;

    std::optional<unsigned int> handleInput(int c);
};

#endif /* OPTIONMENU_H */