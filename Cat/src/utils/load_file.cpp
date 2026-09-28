// TinyObjLoaderによるファイルの読み込み処理
// TinyObjLoaderのためにC++のバージョンはv.17に設定

#pragma warning(push, 0)
#pragma warning(disable: 26495 26498 6287)
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#pragma warning(pop)

#include "load_file.h"
#include <iostream>
#include <limits>

namespace load_file {

    // objファイルの読み込み
    bool load_mesh(
        const std::string& obj_path,
        const std::string& mtl_dir,
        std::vector<Vertex>& out_vertices,
        std::vector<uint32_t>& out_indices,
        glm::vec3& out_size
    ) {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = mtl_dir;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(obj_path, reader_config)) {  // 読み込めなかった場合
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader Error: " << reader.Error() << std::endl;
            }
            return false;
        }

        const auto& attrib = reader.GetAttrib();      // 頂点・UV・法線
        const auto& shapes = reader.GetShapes();      // 面
        const auto& materials = reader.GetMaterials();// マテリアル

        out_vertices.clear();
        out_indices.clear();

        // メモリの事前確保
        size_t total_indices = 0;
        for (const auto& shape : shapes) {
            total_indices += shape.mesh.indices.size();
        }
        out_indices.reserve(total_indices);
        out_vertices.reserve(total_indices / 2); // 重複を考慮して確保

        // Vertexをキーとするハッシュマップ
        std::unordered_map<Vertex, uint32_t> unique_vertices;

        // バウンディングボックスの計算
        glm::vec3 min_pt(std::numeric_limits<float>::max());  // 最小値
        glm::vec3 max_pt(std::numeric_limits<float>::lowest());  // 最大値

        for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
            glm::vec3 pos(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);  // 各頂点の位置
            min_pt = glm::min(min_pt, pos);
            max_pt = glm::max(max_pt, pos);
        }
        out_size = max_pt - min_pt;  // モデルの各軸方向の大きさ

        // メッシュの構築
        for (const auto& shape : shapes) {  // 各物体
            size_t index_offset = 0;

            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {  // 各面
                size_t fv = shape.mesh.num_face_vertices[f];  // 頂点数
                int mat_id = shape.mesh.material_ids[f];  // マテリアルのインデックス

                // マテリアルカラーの設定
                glm::vec3 current_color(0.8f, 0.8f, 0.8f);
                if (mat_id >= 0 && mat_id < static_cast<int>(materials.size())) {
                    current_color = glm::vec3(
                        materials[mat_id].diffuse[0],
                        materials[mat_id].diffuse[1],
                        materials[mat_id].diffuse[2]
                    );
                }

                for (size_t v = 0; v < fv; v++) {  // 各頂点
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];  // 頂点情報の参照インデックス

                    // Vertex構造体を作成
                    Vertex vert{};
                    vert.position = glm::vec3(  // 位置
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2]
                    );

                    if (idx.normal_index >= 0) {  // 法線
                        vert.normal = glm::vec3(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2]
                        );
                    } else {
                        vert.normal = glm::vec3(0.0f);
                    }

                    vert.color = current_color;  // 色

                    // 出力バッファに追加
                    if (unique_vertices.count(vert) == 0) {  // 頂点の重複チェック
                        uint32_t new_idx = static_cast<uint32_t>(out_vertices.size());
                        unique_vertices[vert] = new_idx;  // ハッシュマップに頂点のインデックスを新規追加
                        out_vertices.push_back(vert);
                        out_indices.push_back(new_idx);
                    } else {
                        out_indices.push_back(unique_vertices[vert]);  // ハッシュマップからバッファに追加
                    }
                }
                index_offset += fv;
            }
        }

        return true;
    }
}
