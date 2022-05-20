#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include <stdexcept>
#include <string>

class View {
protected:
    bool needsRefresh;

    bool ableToDisplay;

    unsigned int sizeY;
    unsigned int sizeX;
    bool sizeChanged;
    unsigned int minSizeX;
    unsigned int minSizeY;

    bool warningDisplayed;
    std::string warningText;

    std::string titleText;

    void getWindowSize(WINDOW * forWindow = stdscr);

    unsigned int centeredXInWindow(unsigned int x) const;

    unsigned int centeredYInWindow(unsigned int y) const;

public:
    View();
    virtual ~View();

    virtual void setNeedsRefresh();

    bool isAbleToDisplay();

    virtual void draw(WINDOW * intoWindow = stdscr) = 0;

    virtual View * clone() const = 0;

    void setWarning(bool to, std::string text = "");

    void setTitle(std::string text);

    unsigned int getMinSizeX();
    unsigned int getMinSizeY();
};

struct ViewException : public std::runtime_error {
    ViewException(const std::string & message);
};

#endif /* VIEW_H */