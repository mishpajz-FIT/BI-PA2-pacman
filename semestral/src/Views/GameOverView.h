#ifndef GAMEOVERVIEW_H
#define GAMEOVERVIEW_H

#include "Views/View.h"

class GameOverView : public View {
private:
    unsigned long score;
    unsigned long highscore;
    bool isHighscore;

public:
    GameOverView(unsigned long reachedScore, unsigned long previousHighscore, bool isRecord);

    void draw(WINDOW * intoWindow = stdscr) override;

    GameOverView * clone() const override;
};

#endif /* GAMEOVERVIEW_H */