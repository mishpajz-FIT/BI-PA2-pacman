#ifndef MAINMENUVIEWCONTROLLER_H
#define MAINMENUVIEWCONTROLLER_H

#include "ViewController.h"
#include "LayoutView.h"
#include <memory>
#include "OptionMenu.h"

class MainMenuViewController : public ViewController {

    LayoutView layoutView;

    std::unique_ptr<OptionMenu> menu;

    void handleMenuSelect(unsigned int i);

public:
    MainMenuViewController();

    AppState update() override;

    void draw() override;

};

#endif /* MAINMENUVIEWCONTROLLER_H */