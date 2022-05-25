/****************************************************************
 * @file Position.h
 * @author Michal Dobes
 * @brief Position structure
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef POSITION_H
#define POSITION_H

#include <cmath>

#include "Structures/Transforms/Rotation.h"

/**
 * @brief Two dimensional position
 *
 * Representation of position with whole x and y coordinates.
 *
 */
struct Position {
public:
    int x;
    int y;

    /**
     * @brief Construct a new, nulled Position object
     *
     */
    Position();

    /**
     * @brief Construct a new Position object from coordinates
     *
     * @param newX x coordinate
     * @param newY y coordinate
     */
    Position(int newX, int newY);

    /**
     * @brief Move this position in direction by value
     *
     * @param by value to move position by
     * @param inRotation direction of movement
     * @return Position& this position
     */
    Position & moveBy(int by, Rotation inRotation);

    /**
     * @brief This position moved in direction by value
     *
     * @param by value to move position by
     * @param inRotation direction of movement
     * @return Position copy of this position moved by value
     */
    Position movedBy(int by, Rotation inRotation) const;

    /**
     * @brief Calculate distance between two positions
     *
     * @param lhs
     * @param rhs
     * @return double distance
     */
    static double distanceBetween(const Position & lhs, const Position & rhs);

    bool operator == (const Position & rhs) const;
    bool operator != (const Position & rhs) const;
};

#endif /* POSITION_H */
