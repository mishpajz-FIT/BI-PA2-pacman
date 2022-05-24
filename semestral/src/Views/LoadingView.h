#ifndef LOADINGVIEW_H
#define LOADINGVIEW_H

#include "Views/View.h"

#define LOADINGVIEWFRAMES 5
#define LOADINGVIEWFRAMEHEIGHT 12
#define LOADINGVIEWFRAMEWIDTH 32

class LoadingView : public View {

    static const char frames[LOADINGVIEWFRAMES][LOADINGVIEWFRAMEHEIGHT][LOADINGVIEWFRAMEWIDTH + 1];
    unsigned int currentFrame;
    bool countingUp;

    void nextFrame();

public:
    LoadingView();
    virtual ~LoadingView();

    void draw(WINDOW * intoWindow = stdscr) override;

    LoadingView * clone() const override;
};

#endif /* LOADINGVIEW_H */