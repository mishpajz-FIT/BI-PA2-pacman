/****************************************************************
 * @file Rotation.h
 * @author Michal Dobes
 * @brief Rotation structure
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef ROTATION_H
#define ROTATION_H

#include <stdexcept>

/**
 * @brief Four directional rotation
 *
 */
struct Rotation {
    /**
     * @brief Directions of rotation
     *
     */
    enum Direction : size_t {
        up = 0,
        left,
        down,
        right
    };

    Direction direction; //< Direction of this rotation

    /**
     * @brief Construct a new Rotation object with Rotation::Direction direction
     *
     * @param dir direction
     */
    explicit Rotation(Direction dir);

    /**
     * @brief Construct a new Rotation object, with direction as up
     *
     */
    Rotation();

    /**
     * @brief Construct a new Rotation object with Rotation::Direction numeral value
     *
     * @throws std::invalid_argument Rotation::Direction with value d doesn't exist
     *
     * @param d Value of Rotation::Direction
     */
    Rotation(size_t d);

    /**
     * @brief Rotation with opposite direction than this rotation
     *
     * @return Rotation
     */
    Rotation opposite() const;

    bool operator < (const Rotation & rhs) const;

    bool operator == (const Rotation & rhs) const;

    bool operator != (const Rotation & rhs) const;

};

#endif /* ROTATION_H */
