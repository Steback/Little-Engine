#include "Entity.hpp"


namespace lve {

    Entity::Entity(id_t id, std::string name, Scene *scene) : id(id), name(std::move(name)), scene(scene) {

    }

    Entity::~Entity() = default;

    id_t Entity::getId() const {
        return id;
    }

    const std::string &Entity::getName() const {
        return name;
    }

    void Entity::setName(const std::string &name_) {
        Entity::name = name_;
    }

} // namespace lv