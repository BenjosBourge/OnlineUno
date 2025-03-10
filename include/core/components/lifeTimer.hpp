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

class LifeTimer : public Component {
public:
    LifeTimer();
    LifeTimer(float lifeTime);
    LifeTimer(float lifeTime, bool opacity);
    ~LifeTimer();

    float _lifeTime;
    float _maxLifeTime;
    bool _decreasingOpacity;
};
