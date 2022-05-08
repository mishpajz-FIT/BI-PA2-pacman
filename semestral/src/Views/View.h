#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>

class View {
protected:
    bool needsRefresh;

    int ySize;
    int xSize;
    bool sizeChanged;

    void getWindowSize(WINDOW * forWindow = stdscr);

public:
    View();
    virtual ~View();

    void setNeedsRefresh();

    virtual void draw(WINDOW * intoWindow = stdscr) = 0;
};

#endif /* VIEW_H */