#ifndef SECONDARYVIEW_H
#define SECONDARYVIEW_H

#include "Views/View.h"

class SecondaryView : public View {
protected:
    virtual void drawHint(WINDOW * intoWindow) = 0;

public:
    SecondaryView();
    virtual ~SecondaryView();

};

#endif /* SECONDARYVIEW_H */