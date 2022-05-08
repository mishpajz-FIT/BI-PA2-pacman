#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <ncurses.h>
#include <string>

class ViewController {
protected:
    int termYSize;
    int termXSize;
    bool termSizeChanged;

    bool displaying;

    bool problem;
    bool prevProblem;
    std::string problemMessage;

    virtual void createWindows() = 0;

    void getTermSizes();

    void boxAronudWindow(WINDOW * win);

    void wrongResolution();

public:
    ViewController();

    virtual void update() = 0;

    virtual void draw() = 0;
};

#endif /* STATE_H */