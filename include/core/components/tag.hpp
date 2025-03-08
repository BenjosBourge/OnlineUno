/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <string>
#include "../entity.hpp"
#include "../component.hpp"

class Tag : public Component {
public:
    Tag();
    Tag(std::string tag);
    ~Tag();

    std::string _tag;
};
