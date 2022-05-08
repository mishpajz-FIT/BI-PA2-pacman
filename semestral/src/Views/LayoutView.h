#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include "View.h"

class LayoutView : public View {
protected:
    WINDOW * primaryWindow;
    unsigned int minPrimaryX;
    unsigned int minPrimaryY;

    WINDOW * secondaryWindow;
    const int secondaryX;
    const int minSecondaryY;

    void recreateWindows();

public:
    LayoutView(std::function<void()> refreshCallback);
    virtual ~LayoutView();

    void draw(WINDOW * intoWindow = stdscr) override;

    WINDOW * getPrimaryWindow() const;
    WINDOW * getSecondaryWindow() const;

    void setMinPrimaryWindowDimensions(unsigned int x, unsigned int y);
};

#endif /* LAYOUTVIEW_H */
