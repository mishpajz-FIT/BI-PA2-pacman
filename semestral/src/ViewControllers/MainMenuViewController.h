#ifndef MAINMENUVIEWCONTROLLER_H
#define MAINMENUVIEWCONTROLLER_H

#include <memory>

#include "ViewControllers/ViewController.h"
#include "Views/LayoutView.h"
#include "Utilities/Contexts/OptionMenu.h"

class MainMenuViewController : public ViewController {

    LayoutView layoutView;

    std::unique_ptr<OptionMenu> menu;

    void handleMenuSelect(unsigned int i);

public:
    MainMenuViewController();
    ~MainMenuViewController();

    AppState update() override;

    void draw() override;

};

#endif /* MAINMENUVIEWCONTROLLER_H */