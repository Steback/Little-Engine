#ifndef LITTLEVULKANENGINE_SCENE_HPP
#define LITTLEVULKANENGINE_SCENE_HPP


#include <memory>
#include <string>
#include <unordered_map>

#include "entt/entt.hpp"

#include "utils/NonCopyable.hpp"
#include "files/File.hpp"


namespace lve {

    class Entity;
    class AssetsManager;

    using id_t = entt::entity;

    class Scene : NonCopyable {
        friend class Entity;

    public:
        explicit Scene(AssetsManager& assetsManager);

        ~Scene() override;

        entt::registry& getRegistry();

        Entity* addEntity(const std::string& name);

        Entity* getEntity(id_t id);

        Entity* searchEntity(const std::string& name);

        void load(const std::string& fileNme);

        void save();

    private:
        entt::registry registry;
        std::unordered_map<id_t, std::unique_ptr<Entity>> entities;
        File file;
        AssetsManager& assetsManager;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SCENE_HPP
