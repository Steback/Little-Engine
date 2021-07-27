#include "Entity.hpp"


namespace lve {

    Entity::Entity(id_t id, std::string name, Scene* scene) : id(id), name(std::move(name)), scene(scene) {

    }

    uint32_t Entity::getId() const {
        return static_cast<uint32_t>(id);
    }

    const std::string &Entity::getName() const {
        return name;
    }

    void Entity::setName(const std::string &newName) {
        Entity::name = newName;
    }

} // namespace lve
