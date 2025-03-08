/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include "../component.hpp"

class Transform : public Component {
public:
    Transform();
    Transform(float x, float y);
    Transform(float x, float y, float width, float height);
    ~Transform();

    float _x;
    float _y;
    float _width;
    float _height;
};
