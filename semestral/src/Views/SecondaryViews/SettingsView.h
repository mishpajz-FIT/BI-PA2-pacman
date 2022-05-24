#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include "Views/SecondaryViews/SecondaryView.h"

class SettingsView : public SecondaryView {
protected:
    void drawHint(WINDOW * intoWindow) override;

    bool inputable;

public:
    SettingsView(bool input = true);
    virtual ~SettingsView();

    void draw(WINDOW * intoWindow = stdscr) override;

    SettingsView * clone() const override;
};

#endif /* SETTINGSVIEW_H */
