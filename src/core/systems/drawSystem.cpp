/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

#include "../../../../include/core/systems/drawSystem.hpp"
#include "../../../../include/core/coordinator.hpp"
#include "../../../../include/core/components/transform.hpp"
#include "../../../../include/core/components/spriteRenderer.hpp"
#include "../../../../include/core/components/camera.hpp"
#include "../../../../include/core/components/ui.hpp"
#include "../../../../include/core/ecs.hpp"

DrawSystem::DrawSystem()
{
    _signature = getCoordinator()->getComponentSignature<Transform>();
    _signature |= getCoordinator()->getComponentSignature<SpriteRenderer>();

    if (!shaderFragment.loadFromFile("shader/fragment.frag", sf::Shader::Fragment))
        std::cerr << "Shader not loaded" << std::endl;
}

DrawSystem::~DrawSystem()
{

}

void DrawSystem::update(float deltaTime)
{
    if (isServer())
        return;
    std::shared_ptr<Coordinator> coordinator = getCoordinator();

    float cameraX = 0;
    float cameraY = 0;
    float cameraR = 1;
    float cameraG = 1;
    float cameraB = 1;
    Entity camera = coordinator->getEntityFromTag("camera");
    if (camera != NULL_ENTITY && coordinator->hasComponent<Transform>(camera)
        && coordinator->hasComponent<Camera>(camera)) {
        auto &transform = coordinator->getComponent<Transform>(camera);
        cameraX = transform._x;
        cameraY = transform._y;

        auto &cameraComponent = coordinator->getComponent<Camera>(camera);
        cameraR = (float)cameraComponent._cameraColor.r / 255.0f;
        cameraG = (float)cameraComponent._cameraColor.g / 255.0f;
        cameraB = (float)cameraComponent._cameraColor.b / 255.0f;
    }
    cameraX -= 1920 / 2;
    cameraY -= 1080 / 2;

    for (auto const& entity : _entitiesThisFrame) {
        sf::Sprite sprite;
        auto &transform = coordinator->getComponent<Transform>(entity);
        auto &spriteRenderer = coordinator->getComponent<SpriteRenderer>(entity);

        float r = spriteRenderer._color.r / 255.f;
        float g = spriteRenderer._color.g / 255.f;
        float b = spriteRenderer._color.b / 255.f;

        sprite.setTexture(*(coordinator->getTexture(spriteRenderer._texture)));
        sprite.setTextureRect({spriteRenderer._offsetX, spriteRenderer._offsetY, spriteRenderer._rectWidth, spriteRenderer._rectHeight});
        sprite.setPosition({transform._x - cameraX, transform._y - cameraY + spriteRenderer._yRenderingOffset});
        sprite.setRotation(spriteRenderer._rotation);
        sprite.setOrigin({(float)spriteRenderer._rectWidth / 2, (float)spriteRenderer._rectHeight / 2});
        sprite.setScale({transform._width * (spriteRenderer._isFlippedHorizontally ? -1 : 1), transform._height * (spriteRenderer._isFlippedVertically ? -1 : 1)});

        sf::Glsl::Vec3 colorOverlay(spriteRenderer._colorOverlay.r / 255.f, spriteRenderer._colorOverlay.g / 255.f, spriteRenderer._colorOverlay.b / 255.f);
        sf::Glsl::Vec3 colorMask(spriteRenderer._colorMask.r / 255.f, spriteRenderer._colorMask.g / 255.f, spriteRenderer._colorMask.b / 255.f);
        shaderFragment.setUniform("texture", sf::Shader::CurrentTexture);
        shaderFragment.setUniform("uColor", sf::Glsl::Vec3(r, g, b));
        shaderFragment.setUniform("uColorOverlay", colorOverlay);
        shaderFragment.setUniform("uOverlay", spriteRenderer._overlay ? 1.f : 0.f);
        shaderFragment.setUniform("uColorMask", colorMask);
        shaderFragment.setUniform("uColorMaskOpacity", spriteRenderer._colorMaskOpacity);
        shaderFragment.setUniform("uOpacity", spriteRenderer._opacity);
        coordinator->_window->draw(sprite, &shaderFragment);
    }
}

//TODO : if a movement is done, check the next in the queue, see if the pos in Y has changed

void DrawSystem::addedEntity(Entity entity) //place the entity in the right position in the vector
{
    int indexEnd = _entities.size() - 1;
    int newIndex = 0;
    std::shared_ptr<Coordinator> coordinator = getCoordinator();
    if (!coordinator->hasComponent<SpriteRenderer>(entity) || !coordinator->hasComponent<Transform>(entity))
        return;
    auto &spriteRenderer1 = coordinator->getComponent<SpriteRenderer>(entity);
    auto &transform1 = coordinator->getComponent<Transform>(entity);

    for (int i = indexEnd - 1; i >= 0; i--) {
        auto &spriteRenderer2 = coordinator->getComponent<SpriteRenderer>(_entities[i]);

        if (spriteRenderer1._layer > spriteRenderer2._layer) {
            newIndex = i + 1;
            break;
        } else if (spriteRenderer1._layer == spriteRenderer2._layer) {
            auto &transform2 = coordinator->getComponent<Transform>(_entities[i]);

            if (transform1._y > transform2._y) {
                newIndex = i + 1;
                break;
            }
        }
    }

    for (int i = indexEnd; i > newIndex; i--)
        _entities[i] = _entities[i - 1];
    _entities[newIndex] = entity;
}

void DrawSystem::layerChanged(Entity entity)
{
    int index = NULL_ENTITY;

    for (int i = 0; i < _entities.size(); i++) {
        if (_entities[i] == entity) {
            index = i;
            break;
        }
    }
    if (index == NULL_ENTITY)
        return;
    int i = index;
    for (; i < _entities.size() - 1; i++)
        _entities[i] = _entities[i + 1];
    _entities[i] = entity;
    addedEntity(entity);
}

void DrawSystem::yChanged(Entity entity)
{
    int index = NULL_ENTITY;

    for (int i = 0; i < _entities.size(); i++) {
        if (_entities[i] == entity) {
            index = i;
            break;
        }
    }
    if (index == NULL_ENTITY)
        return;

    int y = getCoordinator()->getComponent<Transform>(entity)._y;
    int layer = getCoordinator()->getComponent<SpriteRenderer>(entity)._layer;
    for (int i = index; i < _entities.size() - 1; i++) {
        int y2 = getCoordinator()->getComponent<Transform>(_entities[i + 1])._y;
        int layer2 = getCoordinator()->getComponent<SpriteRenderer>(_entities[i + 1])._layer;
        if (layer != layer2)
            break;
        if (y > y2) {
            _entities[i] = _entities[i + 1];
            _entities[i + 1] = entity;
            index = i;
        } else
            break;
    }
    for (int i = index; i > 0; i--) {
        int y2 = getCoordinator()->getComponent<Transform>(_entities[i - 1])._y;
        int layer2 = getCoordinator()->getComponent<SpriteRenderer>(_entities[i - 1])._layer;
        if (layer != layer2)
            break;
        if (y < y2) {
            _entities[i] = _entities[i - 1];
            _entities[i - 1] = entity;
        } else
            break;
    }
}

void DrawSystem::removedEntity(Entity entity)
{

}
