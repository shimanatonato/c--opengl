// オブジェクトの表示を管理するクラス

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "src/utils/load_file.h"
#include "Shader.h"

class Mesh
{
    private:
        GLuint m_vao = 0;
        GLuint m_vbo = 0;
        GLuint m_ebo = 0;
        GLsizei m_count;  // indicesの長さ。glDrawElementsで使う
    public:
        Mesh(const std::vector<load_file::Vertex>& vertices, const std::vector<uint32_t>& indices);  // バッファや形状データを設定
        ~Mesh();  // リソースの削除
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&& other) noexcept 
            : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo), m_count(other.m_count) 
        {
            // 移動元の ID を 0 にして無効化（デストラクタで GPU リソースが誤削除されるのを防ぐ）
            other.m_vao = 0;
            other.m_vbo = 0;
            other.m_ebo = 0;
            other.m_count = 0;
        }
        
        void draw() const;  //描画
};

