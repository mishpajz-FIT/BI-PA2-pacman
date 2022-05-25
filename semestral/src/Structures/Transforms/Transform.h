/****************************************************************
 * @file Transform.h
 * @author Michal Dobes
 * @brief Transform structure
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

#include "Structures/Transforms/Position.h"
#include "Structures/Transforms/Rotation.h"

/**
 * @brief Transform combines Position and Rotation
 *
 */
struct Transform {
public:
    Position position;
    Rotation rotation;

    /**
     * @brief Construct a new Transform object
     *
     * @param pos Position
     * @param rot Rotation
     */
    Transform(const Position & pos = Position(), const Rotation & rot = Rotation());

    /**
     * @brief This transform moved in it's rotation's direction by value
     *
     * @param by value to move by
     * @return Transform& this
     */
    Transform & moveBy(int by);

    /**
     * @brief Copy of this tranform moved in it's rotation's direction by value
     *
     * @param by value to move by
     * @return Transform copy of this transform moved by value
     */
    Transform movedBy(int by) const;
};


#endif /* TRANSFORM_H */