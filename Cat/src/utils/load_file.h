// TinyObjLoaderによるファイルの読み込み処理
// TinyObjLoaderのためにC++のバージョンはv.17に設定
#pragma once

#define GLM_ENABLE_EXPERIMENTAL  // GLMのハッシュ機能を有効化
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace load_file {

    // 頂点情報用の構造体
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;

        // 頂点が完全一致するか判定する演算子 (tiny)
        bool operator==(const Vertex& other) const {
            return position == other.position &&
                   normal   == other.normal &&
                   color    == other.color;
        }
    };

    // objファイルの読み込み
    bool load_mesh(
        const std::string& obj_path,
        const std::string& mtl_dir,
        std::vector<Vertex>& out_vertices,
        std::vector<uint32_t>& out_indices,
        glm::vec3& out_size
    );
}

// Vertex構造体をstd::unordered_mapのキーとして使うためのハッシュ関数定義
namespace std {
    template<> struct hash<load_file::Vertex> {
        size_t operator()(load_file::Vertex const& vertex) const {
            size_t h1 = hash<glm::vec3>()(vertex.position);
            size_t h2 = hash<glm::vec3>()(vertex.normal);
            size_t h3 = hash<glm::vec3>()(vertex.color);
            // 各属性のハッシュ値を結合
            return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
        }
    };
}