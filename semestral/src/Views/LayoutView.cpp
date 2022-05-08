#include "LayoutView.h"

void LayoutView::recreateWindows() {
    if (primaryWindow != nullptr) {
        delwin(primaryWindow);
    }
    if (secondaryWindow != nullptr) {
        delwin(secondaryWindow);
    }
    primaryWindow = newwin()
}