#ifndef OPTIONSVIEW_H
#define OPTIONSVIEW_H

#include "View.h"

class OptionsView : public View {
public:
    OptionsView();
    virtual ~OptionsView();

    void draw(WINDOW * intoWindow = stdscr) override;

    OptionsView * clone() const override;
};

#endif /* OPTIONSVIEW_H */
