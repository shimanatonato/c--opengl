// オブジェクト用のシェーダークラス

#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>


class Shader
{
    private:
        GLuint m_id = 0;
        mutable std::unordered_map<std::string, GLint> m_location_cache;  // 行列を転送するロケーションのキャッシュ
        GLint getUniformLocation(const std::string& name) const;  // キャッシュからロケーションを取得、なければ追加
        std::string readShaderFile(const std::string& path);  //ファイルの読み込み
    public:
        bool load(const std::string& vert_path, const std::string& frag_path);  // シェーダーのコンパイル
        ~Shader();
        void use() const;  // このシェーダーを使う
        void setMat4(const std::string& name, const glm::mat4& mat) const;  // 描画時に行列を転送
};    
