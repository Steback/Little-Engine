#include "AssetsManager.hpp"

#include "extras/tiny_obj_loader.h"

#include "graphics/Device.hpp"
#include "files/File.hpp"
#include "files/FilesManager.hpp"
#include "utils/Macros.hpp"
#include "utils/Utils.hpp"


namespace std {

    template <>
    struct hash<lve::Mesh::Vertex> {
        size_t operator()(lve::Mesh::Vertex const &vertex) const {
            size_t seed = 0;
            hash<float> hasher;
            lve::hashCombine(seed, hasher(vertex.position.x), hasher(vertex.position.y), hasher(vertex.position.z));
            lve::hashCombine(seed, hasher(vertex.color.x), hasher(vertex.color.y), hasher(vertex.color.z));
            lve::hashCombine(seed, hasher(vertex.normal.x), hasher(vertex.normal.y), hasher(vertex.normal.z));
            lve::hashCombine(seed, hasher(vertex.uv.x), hasher(vertex.uv.y));
            return seed;
        }
    };

}  // namespace std


namespace lve {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {

    }

    AssetsManager::~AssetsManager() = default;

    void AssetsManager::cleanup() {
        for (auto& [id, mesh] : meshes) mesh->destroy();
    }

    std::pair<std::string, std::shared_ptr<Mesh>> AssetsManager::addMesh(const std::string &fileName) {
        File file = FilesManager::getFile(fileName);
        std::string id = file.getName();
        Mesh::Data data;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.getPath().c_str()))
            LVE_THROW_EX(warn + err);

        std::unordered_map<Mesh::Vertex, uint32_t> uniqueVertices{};
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Mesh::Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2],
                    };

                    auto colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size()) {
                        vertex.color = {
                                attrib.colors[colorIndex - 2],
                                attrib.colors[colorIndex - 1],
                                attrib.colors[colorIndex - 0],
                        };
                    } else {
                        vertex.color = {1.f, 1.f, 1.f};  // set default color
                    }
                }

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                            };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1],
                            };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = CAST_U32(data.vertices.size());
                    data.vertices.push_back(vertex);
                }

                data.indices.push_back(uniqueVertices[vertex]);
            }
        }

        return {id, meshes[id] = std::make_unique<Mesh>(device, data)};
    }

    std::shared_ptr<Mesh> AssetsManager::getMesh(const std::string &id) {
        return meshes[id];
    }

} // namespace lv