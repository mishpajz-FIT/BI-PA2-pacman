/****************************************************************
 * @file GameControl.h
 * @author Michal Dobes
 * @brief Game control context
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#include <ncurses.h>
#include <optional>

#include "Structures/Transforms/Rotation.h"

/**
 * @brief Game control
 *
 * Manages input and converts it into values that can be passed into Game
 *
 */
struct GameControl {
    /**
     * @brief Get player's next rotation from keyboard key
     *
     * W, w, arrow UP - up
     * A, a, arrow LEFT - left
     * s, s, arrow DOWN - down
     * D, d, arrow RIGHT - right
     *
     * @param c value of key
     * @return std::optional<Rotation> Empty if rotation couldn't be decoded from
     *      key, else contains player's next rotation
     */
    static std::optional<Rotation> getPlayerRotationFromKey(int c);
};