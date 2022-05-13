#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include <functional>

class View {
protected:
    bool needsRefresh;

    bool ableToDisplay;

    unsigned int sizeY;
    unsigned int sizeX;
    bool sizeChanged;

    std::function<void()> needsRefreshCallback;

    void getWindowSize(WINDOW * forWindow = stdscr);

public:
    View(std::function<void()> refreshCallback = [ ]() { });
    virtual ~View();

    void setNeedsRefresh();

    bool isAbleToDisplay();

    virtual void draw(WINDOW * intoWindow = stdscr) = 0;
};

#endif /* VIEW_H */