/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

#pragma once
#include "entity.hpp"

class Component {
public:
    Entity _entity;
    virtual ~Component() = default;

    Entity getEntity() {return _entity;}
};
