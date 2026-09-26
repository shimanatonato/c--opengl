#pragma once
#include <opencv2/opencv.hpp>

class OpticalFlowPoint
{
    public:
        OpticalFlowPoint();
        // 前後のフレーム間での動きの中心画素を探す
        // 動きが大きい場合は見つかった中心画素で注目画素を上書き
        // 動きが小さい場合は前フレームの注目画素を引き継ぐ
        bool calcPoint(const cv::Mat& prev, const cv::Mat& next, cv::Vec2i& tgt);
};