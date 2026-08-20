// OpenGLのZバッファー法による3D描画処理クラス

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include "src/utils/load_file.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderObject.h"
#include "src/utils/load_file.h"


class ZBufferRenderer{
    private:
        Shader m_shader;  // オブジェクト用のシェーダー
        Shader m_bg_shader;  // 背景用のシェーダー
        int m_width = 0;  // 画像サイズ
        int m_height = 0;
        GLFWwindow* m_window = nullptr;  // ウィンドウ
        GLuint m_bg_texture_id = 0;  // 背景テクスチャ（カメラ映像フレーム）
        GLuint m_bg_vao = 0;
        GLuint m_bg_vbo = 0;
        glm::mat4 m_proj= glm::mat4(1.0f);  // 射影行列
        glm::mat4 m_view= glm::mat4(1.0f);  // ビュー行列
    public:
        ZBufferRenderer(int width, int height,bool visiblity=true);  // ウィンドウ設定
        void set_camera(const glm::mat4& proj, const glm::mat4& view);  // カメラ設定
        cv::Mat draw_scene(cv::Mat background, const std::vector<RenderObject>& objects);  // オブジェクトを背景に描画
};
