#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <atomic>

// マウス位置の取得
class MouseInput{
    private:
        // 現在のマウス座標
        std::atomic<int> m_x{0};
        std::atomic<int> m_y{0};

        // ウィンドウに紐づけるコールバック
        static void MouseCallback(int event, int x, int y, int flags, void* userdata){
            // マウスが動いたとき座標を更新
            if(event==cv::EVENT_MOUSEMOVE){
                // マウスコールバックにメンバ関数を渡せないのでユーザー定義パラメータ越しに渡す
                auto* self = static_cast<MouseInput*>(userdata);
                self->m_x.store(x);
                self->m_y.store(y);
            }
        }
    public:
        MouseInput() = default;
    
        // ウィンドウの紐づけ
        void RegisterWindow(const std::string& win_name){
            cv::setMouseCallback(win_name,MouseCallback,this);
        }
        // マウス座標の出力
        int GetX() const { return m_x.load(); }
        int GetY() const { return m_y.load(); }
};