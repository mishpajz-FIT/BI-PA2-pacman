#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include "SecondaryView.h"

class SettingsView : public SecondaryView {
protected:
    void drawHint(WINDOW * intoWindow) override;

public:
    SettingsView();
    virtual ~SettingsView();

    void draw(WINDOW * intoWindow = stdscr) override;

    SettingsView * clone() const override;
};

#endif /* SETTINGSVIEW_H */
