#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <ranges>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "src/utils/geometry.h"
#include "src/utils/load_file.h"
#include "src/graphics/ZBufferRenderer.h"
#include "src/config.h"
#include "src/file_path.h"
#include "src/vision/OpticalFlowPoint.h"
#include "src/vision/VideoCaptureWrapper.h"
#include "src/vision/MouseInput.h"
#include "src/game/TargetCircle.h"
#include "src/game/CatController.h"

class App{
    private:
        std::string m_window_name=Config::WINDOW_NAME;
        bool m_is_exit=false;
        
        Config::Mode m_mode=Config::Mode::Video;
        bool m_is_read_target_file=false;  // ターゲット位置をファイルから読み込むか
        bool m_is_use_capture=false;  // 動画・カメラなどOpenCVのキャプチャ機能を使用するか
        bool m_is_use_mouse=false;  // マウス操作をするか
        bool m_is_use_game=false;  // ゲーム機能（ターゲットの生成）をオンにするか

        cv::Size m_image_size;
        glm::mat4 m_proj_mat{1.0f};  // 投影行列
        glm::mat4 m_view_mat{1.0f};  // ビュー行列
        std::vector<std::vector<load_file::Vertex>> m_out_vertice;
        std::vector<std::vector<uint32_t>> m_out_indice;
        float m_scale= 1.0f;  // モデルの大きさの倍率
        
        std::vector<Mesh> m_meshes;  // 3Dメッシュ
        std::vector<RenderObject> m_objs;  // 3Dメッシュとモデル行列の対応

        cv::Mat m_raw_frame;  // 今フレームの各処理前の画像
        cv::Mat m_previmg_gray;  // 前フレームの画像（グレースケール）
        cv::Mat m_fullimg_gray;  // 今フレームのリサイズ前の画像（グレースケール）
        cv::Mat m_nextimg_gray;  // 今フレームの画像（グレースケール）
        cv::Mat m_showimg;  // 表示画像
        cv::Point2f m_focus_px;  // 動きの中心となる画素位置
        cv::Point2f m_head_px;  // 頭の注目画素
        cv::Point2f m_eye_px;  // 目の注目画素

        std::unique_ptr<ZBufferRenderer> m_zbuf_renderer;
        CatController m_cat_controller;
        Game::TargetCircle m_target_circle;
        OpticalFlowPoint m_opt_flow;
        MouseInput m_mouse_input;
        std::unique_ptr<VideoCaptureWrapper> m_video_capture;

    public:
        App() = default;

        void Run(){
            SelectMode();
            // リソース初期化
            if (!InitResources()) {
                std::cerr << "[Error] Failed to initialize App resources." << std::endl;
                return;
            }

            // 時間計測の初期化
            auto last_time = std::chrono::high_resolution_clock::now();
            int frame_sum = 0;
            float time_sum = 0.0f;
            float display_fps = 0.0f;

            // メインループ
            while (!m_is_exit) {
                // 前フレームからの時間差dtの計算
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> delta_time = current_time - last_time;
                float dt = delta_time.count();
                last_time = current_time;

                // メイン処理の実行
                ProcessInput();  // 入力
                Update(dt);  // 処理
                Render();  // 出力

                // デバッグ用 フレームレート出力
                /* frame_sum++;
                time_sum+=dt;
                if (time_sum >= 0.5f) {
                    display_fps = static_cast<float>(frame_sum) / time_sum;
                    time_sum = 0.0f;
                    frame_sum = 0;
                    std::cout << "\rFPS: " << std::fixed << std::setprecision(1) <<  display_fps <<"   " << std::flush;
                }  */              

                // キー入力判定
                // ESCキー（27）が押されたらループを抜ける
                int key = cv::waitKey(1);
                if (key == 27) {
                    m_is_exit = true;
                }
            }
            std::cout << std::endl;
        };
        // モード選択
        void SelectMode();
        // カメラパラメータ、OBJモデル、ウィンドウ、ターゲット情報の初期化
        bool InitResources();
        // 入力（フレーム、マウス位置、オプティカルフロー）処理
        void ProcessInput();
        // 回転計算、ターゲット判定
        void Update(float dt);
        // 3Dレンダリング、2D描画、imshow
        void Render();
};