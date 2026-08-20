// オブジェクト用のシェーダークラス

#include "Shader.h"
#include <fstream>
#include <string>
#include <sstream>

// キャッシュからロケーションを取得、なければ追加
GLint Shader::getUniformLocation(const std::string& name) const{
    // キャッシュ内を検索
    auto it = m_location_cache.find(name);
    if (it != m_location_cache.end()) {
        return it->second; // 見つかったらその値を返す
    }

    // なければOpenGLから取得してキャッシュに保存
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    m_location_cache[name] = loc;
    return loc;
};
// ファイルを文字列として読み込み
std::string Shader::readShaderFile(const std::string& path){
    std::ifstream ifs{path};
    if( !ifs ) throw std::runtime_error("Failed to open file: " + path);
    
    // stringstreamのバッファにより文字列に変換
    std::stringstream buf;
    buf << ifs.rdbuf();
    std::string shader_src{buf.str()};
    return shader_src;
};

// シェーダーのコンパイルとリンク
bool Shader::load(const std::string& vert_path, const std::string& frag_path){
    if (m_id != 0) {
        glDeleteProgram(m_id);
        m_id = 0;
    }
    m_id = glCreateProgram();
    std::string vertex_shader_src = readShaderFile(vert_path);
    std::string fragment_shader_src = readShaderFile(frag_path);
    
    // 頂点シェーダーコンパイル
    const GLuint vsShaderObject = glCreateShader(GL_VERTEX_SHADER);
    const char* vs_src_ptr = vertex_shader_src.c_str();
    glShaderSource(vsShaderObject, 1, &vs_src_ptr, NULL);
    glCompileShader(vsShaderObject);
    glAttachShader(m_id,vsShaderObject);
    
    // フラグメントシェーダーコンパイル
    const GLuint fsShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs_src_ptr = fragment_shader_src.c_str();
    glShaderSource(fsShaderObject, 1, &fs_src_ptr, NULL);
    glCompileShader(fsShaderObject);
    glAttachShader(m_id,fsShaderObject);

    glLinkProgram(m_id);  // リンク
    // 不要なシェーダーオブジェクトの削除
    glDeleteShader(vsShaderObject);
    glDeleteShader(fsShaderObject);
    return true;
};
Shader::~Shader(){
    if (m_id != 0) {
        glDeleteProgram(m_id);
    }
};
// このシェーダーを使う
void Shader::use() const{
    glUseProgram(m_id);
};
// 描画時に行列を転送
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const{
    const GLint loc=getUniformLocation(name);
    glUniformMatrix4fv(loc,1,GL_FALSE,&mat[0][0]);
};
