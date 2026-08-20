// オブジェクトの表示を管理するクラス
#include "Mesh.h"

// バッファや形状データを設定
Mesh::Mesh(const std::vector<load_file::Vertex>& vertices, const std::vector<uint32_t>& indices){
    glGenVertexArrays(1,&m_vao);
    glGenBuffers(1,&m_vbo);
    glGenBuffers(1,&m_ebo);
    // VAOをバインド
    glBindVertexArray(m_vao);
    // VBOの設定
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(load_file::Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );
    // EBOの設定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(uint32_t),
        indices.data(),
        GL_STATIC_DRAW
    );

    // 位置情報 (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                        sizeof(load_file::Vertex), 
                        (void*)offsetof(load_file::Vertex, position));
    glEnableVertexAttribArray(0);

    // 法線情報 (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                        sizeof(load_file::Vertex), 
                        (void*)offsetof(load_file::Vertex, normal));
    glEnableVertexAttribArray(1);

    // 色情報 (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
                        sizeof(load_file::Vertex), 
                        (void*)offsetof(load_file::Vertex, color));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);  // VAOをアンバインド
    m_count = static_cast<GLsizei>(indices.size());
};
// リソースの削除
Mesh::~Mesh(){
    glDeleteVertexArrays(1,&m_vao);
    glDeleteBuffers(1,&m_vbo);
    glDeleteBuffers(1,&m_ebo);
};
//描画
void Mesh::draw() const{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES,m_count,GL_UNSIGNED_INT,(void*)0);
};
