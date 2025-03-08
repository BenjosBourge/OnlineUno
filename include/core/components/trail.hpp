/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include "../texture.hpp"
#include "../component.hpp"

class Trail : public Component {
public:
    Trail();
    Trail(float timerSpawn);
    ~Trail();

    bool _isActive;
    float _timerSpawn;
    float _currentTimerSpawn;
};
