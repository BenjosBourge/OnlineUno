/*------------------------*
 * ECS by Benjamin Bourge *
 *------------------------*/

template<typename T>
T &Coordinator::addComponent(Entity entity)
{
    _componentManager->addComponent<T>(entity);
    _systemManager->entitySignatureChanged(entity, _entityManager->getSignature(entity));

    return getComponent<T>(entity);
}

template<typename T>
T &Coordinator::addComponent(Entity entity, T component)
{
    _componentManager->addComponent<T>(entity, component);
    _systemManager->entitySignatureChanged(entity, _entityManager->getSignature(entity));

    return getComponent<T>(entity);
}

template<typename T>
void Coordinator::removeComponent(Entity entity)
{
    _componentManager->removeComponent<T>(entity);
    _systemManager->entitySignatureChanged(entity, _entityManager->getSignature(entity));
}

template<typename T>
T& Coordinator::getComponent(Entity entity)
{
    return _componentManager->getComponent<T>(entity);
}

template<typename T>
T& Coordinator::getComponent(std::string tag)
{
    return _componentManager->getComponent<T>(getEntityFromTag(tag));
}

template<typename T, typename U>
T& Coordinator::getSibling(U component)
{
    return _componentManager->getComponent<T>(component.getEntity());
}

template<typename T>
bool Coordinator::hasComponent(Entity entity)
{
    Signature signature = _entityManager->getSignature(entity);
    Signature componentSignature = _componentManager->getComponentSignature<T>();
    return (signature & componentSignature) == componentSignature;
}

template<typename T>
Signature Coordinator::getComponentSignature()
{
    return _componentManager->getComponentSignature<T>();
}

template<typename T>
std::vector<Entity> Coordinator::getAllEntitiesWithComponent()
{
    return _componentManager->getAllEntitiesWithComponent<T>();
}