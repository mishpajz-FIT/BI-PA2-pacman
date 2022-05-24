#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <ncurses.h>
#include <string>
#include <AppState.h>

class ViewController {
protected:
    AppState nextState;

    virtual bool handleStateExitKey(int c);

public:
    ViewController();
    virtual ~ViewController();

    virtual AppState update() = 0;

    virtual void draw() = 0;
};

#endif /* STATE_H */