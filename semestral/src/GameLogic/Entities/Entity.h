/****************************************************************
 * @file Entity.h
 * @author Michal Dobes
 * @brief Model of entity
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef ENTITY_H
#define ENTITY_H

#include <tuple>

#include "Structures/Transforms/Transform.h"
#include "Utilities/NCColors.h"

/**
 * @brief Entity
 *
 */
class Entity {
protected:
    Transform transform; //< Current transform (rotation and position)
    Rotation nextRotation; //< Rotation that can be used in movement
    bool alive;

public:

    /**
     * @brief Construct a new Entity
     *
     * @param initial Initial transform
     * @param a Alive
     */
    Entity(const Transform & initial, bool a = false);

    /**
     * @brief Destroy the Entity
     *
     */
    virtual ~Entity();

    /**
     * @brief Change transform of entity to completely other one
     *
     * @param trans Target transform
     */
    void reposition(const Transform & trans);

    /**
     * @brief Get transform of entity
     *
     * @return Transform
     */
    Transform getTransform() const;

    /**
     * @brief Toggle alive on/off
     *
     * Kills/resurrects entity
     *
     */
    void toggleAlive();

    /**
     * @brief Is entity alive
     *
     * @return true
     * @return false
     */
    bool isAlive();

    /**
     * @brief Get display information used for displaying the entity
     *
     * Gets char and color information
     *
     * @return std::pair<char, NCColors::ColorPairs> pair with char that represents entity and color pair
     */
    virtual std::pair<char, NCColors::ColorPairs> displayEntity() = 0;
};

#endif /* ENTITY_H */