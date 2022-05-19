#include "LoadingView.h"
#include "NCColors.h"

const char LoadingView::frames[LOADINGVIEWFRAMES][LOADINGVIEWFRAMEHEIGHT][LOADINGVIEWFRAMEWIDTH + 1] = {
    {
        "        ##############         ",
        "     ####################      ",
        "  ################## ########  ",
        " ##################   ######## ",
        " ################### ######### ",
        "##############################-",
        "##############################-",
        " ############################# ",
        " ############################# ",
        "  ###########################  ",
        "     ####################      ",
        "        ##############         "
    },
    {
        "        ##############         ",
        "     ####################      ",
        "  ################## ########  ",
        " ##################   ######## ",
        " ################### ########/ ",
        "############################|  ",
        "############################|  ",
        " ############################\\ ",
        " ############################# ",
        "  ###########################  ",
        "     ####################      ",
        "        ##############         ",
    },
    {
        "        ##############         ",
        "     ####################      ",
        "  ################## ########  ",
        " ##################   #######/ ",
        " ################### #######/  ",
        "###########################|   ",
        "###########################|   ",
        " ###########################\\  ",
        " ############################\\ ",
        "  ###########################  ",
        "     ####################      ",
        "        ##############         "

    },
    {
        "        ##############         ",
        "     ####################      ",
        "  ################## #######/  ",
        " ##################   ####/    ",
        " ################### ###/      ",
        "######################|        ",
        "######################|        ",
        " #######################\\      ",
        " #########################\\    ",
        "  ##########################\\  ",
        "     ####################      ",
        "        ##############         "
    },
    {
        "        ##############         ",
        "     ####################      ",
        "  ################## ######/   ",
        " ##################   ##/      ",
        " ################### #/        ",
        "####################|          ",
        "####################|          ",
        " #####################\\        ",
        " #######################\\      ",
        "  #########################\\   ",
        "     ####################      ",
        "        ##############         "
    }
};

void LoadingView::nextFrame() {
    if (countingUp) {
        currentFrame++;
    } else {
        currentFrame--;
    }

    if (currentFrame == 0) {
        countingUp = true;
    } else if (currentFrame == LOADINGVIEWFRAMES - 1) {
        countingUp = false;
    }

    setNeedsRefresh();
}

LoadingView::LoadingView() : View(), currentFrame(0), countingUp(true) {
    minSizeX = LOADINGVIEWFRAMEWIDTH;
    minSizeY = LOADINGVIEWFRAMEHEIGHT;
}

void LoadingView::draw(WINDOW * intoWindow) {
    nextFrame();
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    wattron(intoWindow, COLOR_PAIR(NCColors::pacman));
    for (size_t i = 0; i < LOADINGVIEWFRAMEHEIGHT; i++) {
        mvwprintw(intoWindow, centeredYInWindow(i + 1), centeredXInWindow(1), frames[currentFrame][i]);
    }
    wattroff(intoWindow, COLOR_PAIR(NCColors::pacman));

    wrefresh(intoWindow);
    needsRefresh = false;
}

LoadingView::~LoadingView() { }

LoadingView * LoadingView::clone() const {
    return new LoadingView(*this);
}