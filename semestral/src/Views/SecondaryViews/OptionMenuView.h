#ifndef MENUVIEW_H
#define MENUVIEW_H

#include "SecondaryView.h"
#include "OptionMenu.h"
#include <string>
#include <vector>

class OptionMenuView : public SecondaryView {
protected:
    OptionMenu * menuToDraw;

    void drawHint(WINDOW * intoWindow) override;

public:
    OptionMenuView(OptionMenu * menu);
    virtual ~OptionMenuView();

    void draw(WINDOW * intoWindow = stdscr) override;

    OptionMenuView * clone() const override;
};

#endif /* MENUVIEW_H */