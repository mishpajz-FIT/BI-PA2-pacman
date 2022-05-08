#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <ncurses.h>
#include <string>

class ViewController {
public:
    ViewController();

    virtual void update() = 0;

    virtual void draw() = 0;
};

#endif /* STATE_H */