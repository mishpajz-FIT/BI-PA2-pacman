#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include "View.h"
#include <memory>

class LayoutView : public View {
protected:
    WINDOW * primaryWindow;
    std::unique_ptr<View> primaryView;

    WINDOW * secondaryWindow;
    std::unique_ptr<View> secondaryView;

    void recreateWindows();

public:
    LayoutView();
    LayoutView(const LayoutView & toCopy);
    virtual ~LayoutView();

    void draw(WINDOW * intoWindow = stdscr) override;

    void setNeedsRefresh() override;

    WINDOW * getPrimaryWindow() const;
    WINDOW * getSecondaryWindow() const;

    View * getPrimaryView() const;
    View * getSecondaryView() const;

    void setPrimaryView(const View & view);
    void setSecondaryView(const View & view);

    LayoutView * clone() const override;
};

#endif /* LAYOUTVIEW_H */
