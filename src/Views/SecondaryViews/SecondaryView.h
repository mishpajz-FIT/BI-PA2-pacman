/****************************************************************
 * @file SecondaryView.h
 * @author Michal Dobes
 * @brief Secondary view
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef SECONDARYVIEW_H
#define SECONDARYVIEW_H

#include "Views/View.h"

/**
 * @brief Secondary view
 *
 */
class SecondaryView : public View {
protected:

    /**
     * @brief Draw defined hints into window
     *
     * @param intoWindow window to draw hint into
     */
    virtual void drawHint(WINDOW * intoWindow) = 0;

public:

    /**
     * @brief Construct a new Secondary View object
     *
     */
    SecondaryView();

    /**
     * @brief Destroy the Secondary View object
     *
     */
    virtual ~SecondaryView();

};

#endif /* SECONDARYVIEW_H */