#ifndef LITTLEVULKANENGINE_SCENE_HPP
#define LITTLEVULKANENGINE_SCENE_HPP


#include <memory>
#include <string>
#include <unordered_map>

#include "entt/entt.hpp"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Entity;

    using id_t = entt::entity;

    class Scene : NonCopyable {
        friend class Entity;

    public:
        Scene();

        ~Scene() override;

        entt::registry& getRegistry();

        Entity* addEntity(const std::string& name);

        Entity* getEntity(id_t id);

    private:
        entt::registry registry;
        std::unordered_map<id_t, std::unique_ptr<Entity>> entities;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SCENE_HPP
