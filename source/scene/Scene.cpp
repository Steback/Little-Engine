#include "Scene.hpp"

#include "Entity.hpp"


namespace lve {

    Scene::Scene() = default;

    Entity *Scene::addEntity(const std::string &name) {
        id_t id = registry.create();
        entities[id] = std::make_unique<Entity>(id, name, this);
        return entities[id].get();
    }

    Entity *Scene::getEntity(id_t id) {
        return entities[id].get();
    }

    const std::unordered_map<id_t, std::unique_ptr<Entity>> &Scene::getEntities() const {
        return entities;
    }

    registry_t &Scene::getRegistry() {
        return registry;
    }

} // namespace lve
