#ifndef GAMEDETAILVIEW_H
#define GAMEDETAILVIEW_H

#include "View.h"

class GameDetailView : public View {
public:
    GameDetailView();
    virtual ~GameDetailView();

    void draw(WINDOW * intoWindow = stdscr) override;

    void setWarning(bool to, std::string text = "") override;

    GameDetailView * clone() const override;
};

#endif /* GAMEDETAILVIEW_H */