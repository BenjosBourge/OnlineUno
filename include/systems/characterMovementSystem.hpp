/*-----------------------------*
 * Etipi by Benjamin Bourge *
 *-----------------------------*/

#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include "../core/system.hpp"

class CharacterMovementSystem : public System {
public:
    CharacterMovementSystem();
    ~CharacterMovementSystem();

    void update(float deltaTime) override;
    void addedEntity(Entity entity) override;
    void removedEntity(Entity entity) override;
};
