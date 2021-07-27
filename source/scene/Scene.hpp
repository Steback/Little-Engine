#ifndef LITTLEVULKANENGINE_SCENE_HPP
#define LITTLEVULKANENGINE_SCENE_HPP


#include <memory>
#include <unordered_map>

#include "entt/entt.hpp"
#include "vulkan/vulkan.hpp"


namespace lve {

    using id_t = entt::entity;

    class Entity;

    class Scene {
        friend Entity;

    public:
        Scene();

        Entity* addEntity(const std::string& name);

        Entity* getEntity(id_t id);

        const std::unordered_map<id_t, std::unique_ptr<Entity>>& getEntities() const;

    private:
        std::unordered_map<id_t, std::unique_ptr<Entity>> entities;
        entt::registry registry;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SCENE_HPP
