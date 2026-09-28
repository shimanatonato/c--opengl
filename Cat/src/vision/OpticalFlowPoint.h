#pragma once
#include <opencv2/opencv.hpp>
#include "src/config.h"

class OpticalFlowPoint
{
    private:
        cv::Mat m_flow;
        float m_min_sq=Config::OPTFLOW_MOVEMIN*Config::OPTFLOW_MOVEMIN;  // 閾値（フローの2乗）
        float m_flow_exp=Config::OPTFLOW_EXP*0.5f;  // 動きの大きい画素の重みをより重くするためのべき乗の指数
        float m_flow_pixmin=Config::OPTFLOW_PIXMIN;  // フローの大きさが閾値を超える画素数の最低値

        // Farnebackオプティカルフローのパラメータ
        double m_pyr_scale = 0.5;   // pyrScale     : 画像ピラミッドの前の層に対するスケール
        int m_levels = 3;           // levels       : 画像ピラミッドの層数
        int m_winsize = 15;         // winsize      : 平均化の窓サイズ
        int m_iterations = 3;       // iterations   : 画像ピラミッド各層のアルゴリズム反復数
        int m_poly_n = 5;           // polyN        : ピクセル近傍領域のサイズ
        double m_poly_sigma = 1.2;  // polySigma    : ガウス分布の標準偏差(polyN=5 ならば polySigma=1.1あたり)    
    public:
        OpticalFlowPoint()=default;
        // 前後のフレーム間での動きの中心画素を探す
        // 動きが大きい場合は見つかった中心画素を出力
        // 動きが小さい場合はfalseを返す
        bool calcPoint(const cv::Mat& prev, const cv::Mat& next, cv::Point2f& tgt);
};