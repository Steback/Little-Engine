#include "Scene.hpp"

#include "entity/Entity.hpp"


namespace lve {

    Scene::Scene() = default;

    Scene::~Scene() = default;

    entt::registry &Scene::getRegistry() {
        return registry;
    }

    Entity *Scene::addEntity(const std::string &name) {
        id_t id = registry.create();
        entities[id] = std::make_unique<Entity>(id, name, this);
        return entities[id].get();
    }

    Entity *Scene::getEntity(id_t id) {
        return entities[id].get();
    }

} // namespace lv