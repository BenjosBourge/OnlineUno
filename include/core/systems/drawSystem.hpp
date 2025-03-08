/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>
#include "../system.hpp"

class DrawSystem : public System {
public:
    DrawSystem();
    ~DrawSystem();

    void update(float deltaTime) override;
    void addedEntity(Entity entity) override;
    void removedEntity(Entity entity) override;

    void layerChanged(Entity entity);
    void yChanged(Entity entity);

    sf::Shader shaderFragment;
    sf::Shader shaderVertex;
};
