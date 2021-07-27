#ifndef LITTLEVULKANENGINE_ENTITY_HPP
#define LITTLEVULKANENGINE_ENTITY_HPP


#include <string>

#include "entt/entt.hpp"

#include "Scene.hpp"


namespace lve {

    class Scene;

    class Entity {
    public:
        Entity(id_t id, std::string name, Scene* scene);

        [[nodiscard]] uint32_t getId() const;

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &newName);

        template<typename T, typename... Args>
        T& addComponent(Args&& ...args);

        template<typename T>
        T& getComponent();

        template<typename T>
        bool hasComponent();

    private:
        id_t id;
        std::string name;
        Scene* scene;
    };

    template<typename T, typename... Args>
    T& Entity::addComponent(Args&& ...args) {
        return scene->registry.emplace<T>(id, std::forward<Args>(args)...);
    }

    template<typename T>
    T& Entity::getComponent() {
        return scene->registry.get<T>(id);
    }

    template<typename T>
    bool Entity::hasComponent() {
        return scene->registry.any_of<T>(id);
    }

} // namespace lve


#endif //LITTLEVULKANENGINE_ENTITY_HPP
