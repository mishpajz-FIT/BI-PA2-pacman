#ifndef OPTIONSVIEW_H
#define OPTIONSVIEW_H

#include "View.h"
#include <string>

class OptionsView : public View {
protected:
    bool warningDisplayed;
    std::string warningText;

    std::string titleText;

    bool inputting;

public:
    OptionsView();
    virtual ~OptionsView();

    void draw(WINDOW * intoWindow = stdscr) override;

    void setWarning(bool to, std::string text = "");

    void setTitle(std::string text);

    void setInput(bool to);
};

#endif /* OPTIONSVIEW_H */
