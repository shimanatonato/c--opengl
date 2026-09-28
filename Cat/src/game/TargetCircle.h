#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "src/config.h"

namespace Game{
    struct TargetInfo {
        int x, y, r;  // 中心位置と半径
    };
    class TargetCircle{
        private:
            cv::Size m_image_size;
            // 短辺に対するターゲットサイズの比率のランダム範囲
            float m_r_max=Config::TARGET_RADIUS_MAX;
            float m_r_min=Config::TARGET_RADIUS_MIN;
            // ターゲット色
            cv::Scalar m_color_default=Config::TARGET_COLOR;
            cv::Scalar m_color_cleared=Config::TARGET_COLOR_CLEARED;
            float m_alpha=Config::TARGET_ALPHA;

            // 現在のターゲット情報
            TargetInfo m_current_target{0,0,0};
            // ターゲット位置をファイルから読み込む場合
            bool m_is_read_target_file=false;  // ファイル読み込みの有無
            std::vector<TargetInfo> m_targets_list;  // ターゲットのリスト
            int m_target_file_ind=-1;  // 表示中ターゲットのインデックス

            // フラグ
            bool m_is_exist_target=false;  // ターゲットが存在中か
            bool m_is_reached_target=false;  // ターゲットに到達済みか
            float m_reached_target_rest_time_s=0.0f;  // ターゲットが消えるまでの秒数
            
            // 到達判定
            bool IsInside(const float x, const float y) const;
            // ターゲットを新しく生成
            void SpawnCircle();
        public:
            TargetCircle()=default;
            TargetCircle(const cv::Size& size);
            // ファイルの読み込み
            bool LoadTargetFile(const std::string& source);
            // 画像にターゲットを描画
            void Draw(cv::Mat& out_img);
            // 状態の更新
            void Update(const float dt, const cv::Point2f& focus_px);

            // フラグの出力
            bool IsExist() const;
            bool IsReached() const;
    };
}