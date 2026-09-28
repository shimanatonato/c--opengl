#include "OpticalFlowPoint.h"

bool OpticalFlowPoint::calcPoint(const cv::Mat& prev, const cv::Mat& next, cv::Point2f& tgt)
{
    if (prev.empty() || next.empty()) {
        return false;
    }
    if (prev.size() != next.size() || prev.type() != CV_8UC1 || next.type() != CV_8UC1) {
        return false;
    }
    // オプティカルフローを計算
    cv::calcOpticalFlowFarneback(
        prev,next,m_flow,
        m_pyr_scale,
        m_levels,
        m_winsize,
        m_iterations,
        m_poly_n,
        m_poly_sigma,
        0       // flags        : 処理フラグ
    );

    // フローの大きさが閾値以上の画素をカウント
    uint count_passed_pix=0;  // 閾値以上の画素数
    // フローの大きさを重みとして各画素の座標を加重平均し動きの中心となる画素を探す
    float sum_x=0.0f;
    float sum_y=0.0f;
    float weight=0.0f;
    
    // 各画素を走査
    for(int iy = 0; iy<m_flow.size[0]; iy++){
            const cv::Vec2f* rowptr = m_flow.ptr<cv::Vec2f>(iy);
            for(int ix = 0; ix<m_flow.size[1]; ix++){
                float norm_sq = rowptr[ix][0]*rowptr[ix][0] + rowptr[ix][1]*rowptr[ix][1];  // フローの大きさ（2乗）
                if (norm_sq<m_min_sq) continue;  // 閾値以下の画素を除去
                count_passed_pix++;  // 閾値以上の画素をカウント
                
                // べき乗して動きの大きい画素の重みをより重くする
                float power=static_cast<float>(std::pow(norm_sq,m_flow_exp));
                sum_x+=static_cast<float>(ix)*power;
                sum_y+=static_cast<float>(iy)*power;
                weight+=power;
            }
    }
    // フローの大きい画素が少ない場合（=動きが小さい場合）
    // 処理終了
    if (count_passed_pix<m_flow_pixmin) return false;
    
    // 一定以上の画素が閾値を超えた場合（=動きが大きい場合）
    // 動きの中心画素を上書き
    if (weight > 0.0f){
        tgt.x=sum_x/weight;
        tgt.y=sum_y/weight;
        return true;
    }
    else return false;

}