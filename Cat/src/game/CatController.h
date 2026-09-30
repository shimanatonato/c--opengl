#pragma once
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include "src/config.h"

struct CatPose {
    glm::mat4 head{1.0f};
    glm::mat4 left_eye{1.0f};
    glm::mat4 right_eye{1.0f};
};

class CatController
{
    private:
        float m_scale= 1.0f;  // スケール倍率
        glm::mat4 m_scale_mat{1.0f};  // スケーリング行列
        glm::mat4 m_offset_center_mat{1.0f};  // オブジェクト全体の中心位置の補正
        glm::vec3 m_center_leye=Config::LEFT_EYE_CENTER;  // 左目オブジェクトの中心
        glm::vec3 m_center_reye=Config::RIGHT_EYE_CENTER;  // 右目オブジェクトの中心

        // 注目している2次元座標を3次元座標に変換
        glm::vec3 PixelTo3D(const cv::Point2f& p2d, const cv::Size& img_size) const;
    public:
        glm::mat4 m_rot_init{1.0f};  // モデルの向きを世界座標系基準に合わせる回転
        CatController();
        
        // スケーリング設定
        void setScale(const float scale);

        // モデル行列の計算
        // 各オブジェクトの注目位置は(座標/画像サイズ)で与える
        CatPose CalcCatPose(const cv::Point2f& head_px, const cv::Point2f& eye_px, const cv::Size& img_size) const;
};