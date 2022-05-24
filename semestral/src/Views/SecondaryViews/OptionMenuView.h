#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <string>
#include <vector>

#include "Views/SecondaryViews/SecondaryView.h"
#include "Utilities/Contexts/OptionMenu.h"

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