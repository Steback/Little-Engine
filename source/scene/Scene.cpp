#include "Scene.hpp"

#include "entity/Entity.hpp"
#include "utils/Json.hpp"
#include "files/FilesManager.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshInterface.hpp"


namespace lve {

    Scene::Scene(AssetsManager& assetsManager) : assetsManager(assetsManager) {

    }

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

    Entity *Scene::searchEntity(const std::string &name) {
        for (auto& [id, entity] : entities) {
            if (entity->getName() == name) return entity.get();
        }

        return nullptr;
    }

    void Scene::load(const std::string& fileNme) {
        file = FilesManager::getFile(fileNme);

        json data;
        file.read(data);

        for (auto& entityData : data["entities"]) {
            auto* entity = addEntity(entityData["name"]);

            if (!entityData["transform"].empty()) {
                auto& transform = entityData["transform"];
                auto& translation = transform["translation"];
                auto& rotation = transform["rotation"];
                auto& scale = transform["scale"];
                entity->addComponent<Transform>(
                        vec3{translation["x"].get<float>(), translation["y"].get<float>(), translation["z"].get<float>()},
                        vec3{rotation["x"].get<float>(), rotation["y"].get<float>(), rotation["z"].get<float>()},
                        vec3{scale["x"].get<float>(), scale["y"].get<float>(), scale["z"].get<float>()}
                        );
            }

            if (!entityData["meshInterface"].empty()) {
                auto mesh = assetsManager.addMesh("models/" + entityData["meshInterface"]["name"].get<std::string>());
                entity->addComponent<MeshInterface>(mesh.first, mesh.second);
            }
        }
    }

    void Scene::save() {
        file.remove();

        json data;
        data["entities"] = {};

        for (auto& [id, entity] : entities) {
            size_t entitiesCount = data["entities"].size();

            data["entities"].push_back({
                {"name", entity->getName()},
            });

            if (entity->hasComponent<Transform>()) {
                auto& transform = registry.get<Transform>(id);
                data["entities"][entitiesCount]["transform"] = {
                        { "translation", {
                            {"x", transform.getTranslation().x},
                            {"y", transform.getTranslation().y},
                            {"z", transform.getTranslation().z}
                        } },
                        { "rotation", {
                            {"x", transform.getRotation().x},
                            {"y", transform.getRotation().y},
                            {"z", transform.getRotation().z}
                        } },
                        { "scale", {
                            {"x", transform.getScale().x},
                            {"y", transform.getScale().y},
                            {"z", transform.getScale().z}
                        } }
                };
            }

            if (entity->hasComponent<MeshInterface>()) {
                auto& meshInterface = registry.get<MeshInterface>(id);
                data["entities"][entitiesCount]["meshInterface"] = {
                        {"name", meshInterface.getId()}
                };
            }
        }

        file.write(data);
    }

} // namespace lv