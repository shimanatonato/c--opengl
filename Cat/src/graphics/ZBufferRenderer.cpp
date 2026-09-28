// OpenGLのZバッファー法による3D描画処理クラス

# include "ZBufferRenderer.h"
# include "src/file_path.h"
#include <ranges>

// ウィンドウ設定
ZBufferRenderer::ZBufferRenderer(int width, int height,bool visiblity)
{
    m_width = width;
    m_height = height;
    if(!visiblity) glfwWindowHint(GLFW_VISIBLE,0);  // ウィンドウの非表示
    // ウィンドウ作成
    m_window = glfwCreateWindow(m_width, m_height, "Drawing CG", NULL, NULL);
    if (m_window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // 深度テスト（Zバッファ）の有効化
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glViewport(0, 0, m_width, m_height);  // 画面ピクセルサイズを設定

    // シェーダープログラムを読み込み
    m_shader.load(FilePath::VERTEX_SRC_PATH, FilePath::FRAGMENT_SRC_PATH);
    m_bg_shader.load(FilePath::BG_VERTEX_SRC_PATH, FilePath::BG_FRAGMENT_SRC_PATH);
    
    // 背景テクスチャ設定
    glGenTextures(1, &m_bg_texture_id);
    glBindTexture(GL_TEXTURE_2D,m_bg_texture_id);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //背景ポリゴン設定
    float bg_vertices[] = {
        -1.0f, -1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 0.0f
    };
    glGenVertexArrays(1,&m_bg_vao);
    glGenBuffers(1,&m_bg_vbo);
    glBindVertexArray(m_bg_vao);
    glBindBuffer(GL_ARRAY_BUFFER,m_bg_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(bg_vertices),
        bg_vertices,
        GL_STATIC_DRAW
    );

    // 位置情報 (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
                        4 * sizeof(float), 
                        (void*)0);
    glEnableVertexAttribArray(0);

    // テクスチャ情報 (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
                        4 * sizeof(float), 
                        (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);  // VAOをアンバインド     
};
// カメラ設定
void ZBufferRenderer::set_camera(const glm::mat4& proj, const glm::mat4& view){
    m_proj = proj;
    m_view = view;
};
// オブジェクトを背景に描画
cv::Mat ZBufferRenderer::draw_scene(cv::Mat background, const std::vector<RenderObject>& objects){
    glfwPollEvents();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glBindTexture(GL_TEXTURE_2D,m_bg_texture_id);  //背景テクスチャのバインド
    // 背景画像のテクスチャ化
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
        m_width, m_height, 0, 
        GL_BGR, GL_UNSIGNED_BYTE, background.data);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // 背景描画
    glDisable(GL_DEPTH_TEST);  // 深度テストをオフにして、一番奥に描かれるようにする
    m_bg_shader.use();
    glBindVertexArray(m_bg_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0); // アンバインド

    // 3Dオブジェクトの描画
    glEnable(GL_DEPTH_TEST);
    m_shader.use();
    m_shader.setMat4("u_projection",m_proj);
    m_shader.setMat4("u_view",m_view);

    // 各オブジェクトに入力したモデル行列を適用
    auto valid_objects=objects|std::views::filter([](const RenderObject& o) { return o.mesh != nullptr; });
    for (const auto& obj : valid_objects) {
        m_shader.setMat4("u_model", obj.model_matrix);
        obj.mesh->draw();
    }

    // OpenCV行列に出力
    cv::Mat res_image(m_height, m_width, CV_8UC3);
    glReadPixels(0,0,m_width,m_height,GL_BGR,GL_UNSIGNED_BYTE,res_image.data);
    cv::flip(res_image,res_image,0);
    glfwSwapBuffers(m_window);
    return res_image;
};

ZBufferRenderer::~ZBufferRenderer(){
    // 背景のVBO,VAOの削除（その他物体はMeshクラスのデストラクタで削除）
    if (m_bg_vbo != 0) {
        glDeleteBuffers(1, &m_bg_vbo);
        m_bg_vbo = 0;
    }
    if (m_bg_vao != 0) {
        glDeleteVertexArrays(1, &m_bg_vao);
        m_bg_vao = 0;
    }
    // テクスチャの削除
    if (m_bg_texture_id != 0) {
        glDeleteTextures(1, &m_bg_texture_id);
        m_bg_texture_id = 0;
    }
    // GLFWウィンドウの破棄
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
};