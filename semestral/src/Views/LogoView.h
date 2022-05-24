#ifndef LOGOVIEW_H
#define LOGOVIEW_H

#include "Views/View.h"

#define LOGOVIEWLOGOWIDTH 44
#define LOGOVIEWLOGOHEIGHT 5 

class LogoView : public View {
    static const char logo[LOGOVIEWLOGOHEIGHT][LOGOVIEWLOGOWIDTH];

public:
    LogoView();
    virtual ~LogoView();

    void draw(WINDOW * intoWindow = stdscr) override;

    LogoView * clone() const override;
};

#endif /* LOGOVIEW_H */