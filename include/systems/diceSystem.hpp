/*-----------------------------*
 * Etipi by Benjamin Bourge *
 *-----------------------------*/

#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include "../core/system.hpp"

class DiceSystem : public System {
public:
    DiceSystem();
    ~DiceSystem();

    void update(float deltaTime) override;
    void addedEntity(Entity entity) override;
    void removedEntity(Entity entity) override;

    Entity _selectedDice;
    int _offsetX;
    int _offsetY;
};
