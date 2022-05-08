#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include "View.h";

class LayoutView : public View {
protected:
    WINDOW * primaryWindow;
    unsigned int minPrimaryX;
    unsigned int minPrimaryY;

    WINDOW * secondaryWindow;

    void recreateWindows();

public:
    LayoutView();
    virtual ~LayoutView();

    void draw(WINDOW * intoWindow = stdscr) override;

    WINDOW * getPrimaryWindow();
    WINDOW * getSecondaryWindow();

    void setMinPrimaryWindowDimensions(unsigned int x, unsigned int y);
};

#endif /* LAYOUTVIEW_H */
