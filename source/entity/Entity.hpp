#ifndef LITTLEVULKANENGINE_ENTITY_HPP
#define LITTLEVULKANENGINE_ENTITY_HPP


#include "scene/Scene.hpp"


namespace lve {

    class Entity {
    public:
        Entity(id_t id, std::string  name, Scene* scene);

        ~Entity();

        [[nodiscard]] id_t getId() const;

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        template<typename T, typename ...Args>
        T& addComponent(Args&& ...args);

        template<typename T>
        T& getComponent();

        template<typename T>
        bool hasComponent();

    private:
        id_t id{};
        std::string name{};
        Scene* scene{};
        uint32_t components{};
    };

    template<typename T, typename ...Args>
    T &Entity::addComponent(Args&& ...args) {
        components |= T::Type;
        return scene->registry.emplace<T>(id, std::forward<Args>(args)...);
    }

    template<typename T>
    T &Entity::getComponent() {
        return scene->registry.get<T>(id);
    }

    template<typename T>
    bool Entity::hasComponent() {
        return components & T::Type;
    }

} // namespace lve


#endif //LITTLEVULKANENGINE_ENTITY_HPP
