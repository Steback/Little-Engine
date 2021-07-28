#ifndef LITTLEVULKANENGINE_SCENE_HPP
#define LITTLEVULKANENGINE_SCENE_HPP


#include <memory>
#include <unordered_map>

#include "entt/entt.hpp"
#include "vulkan/vulkan.hpp"


namespace lve {

    using registry_t = entt::registry;
    using id_t = entt::entity;

    class Entity;

    class Scene {
        friend Entity;

    public:
        Scene();

        Entity* addEntity(const std::string& name);

        Entity* getEntity(id_t id);

        const std::unordered_map<id_t, std::unique_ptr<Entity>>& getEntities() const;

        registry_t& getRegistry();

    private:
        std::unordered_map<id_t, std::unique_ptr<Entity>> entities;
        registry_t registry;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SCENE_HPP
