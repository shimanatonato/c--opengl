#include "OpticalFlowPoint.h"
#include "src/config.h"

OpticalFlowPoint::OpticalFlowPoint()
{
}

bool OpticalFlowPoint::calcPoint(const cv::Mat& prev, const cv::Mat& next, cv::Vec2i& tgt)
{
    // オプティカルフローを計算
    cv::Mat flow;
    cv::calcOpticalFlowFarneback(
        prev,next,flow,
        0.5,    // pyrScale     : 画像ピラミッドの前の層に対するスケール
        3,      // levels       : 画像ピラミッドの層数
        15,     // winsize      : 平均化の窓サイズ
        3,      // iterations   : 画像ピラミッド各層のアルゴリズム反復数
        5,      // polyN        : ピクセル近傍領域のサイズ
        1.2,    // polySigma    : ガウス分布の標準偏差(polyN=5 ならば polySigma=1.1あたり)
        0       // flags        : 処理フラグ
    );

    // フローの大きさが閾値以上の画素を集計
    uint count_passed_pix=0;
    size_t total_pixels = flow.total();
    const cv::Vec2f* ptr = flow.ptr<cv::Vec2f>(0);
    float min_sq=Config::OPTFLOW_MOVEMIN*Config::OPTFLOW_MOVEMIN;
    for (size_t i = 0; i < total_pixels; ++i) {
        // 各画素を走査
        float norm_sq = ptr[i][0]*ptr[i][0] + ptr[i][1]*ptr[i][1];
        if (norm_sq>=min_sq) count_passed_pix++;
    }
    
    if (count_passed_pix>=Config::OPTFLOW_PIXMIN){
        // 一定以上の画素が閾値を超えた場合（動きが大きい場合）
        // フローの大きさを重みとして各画素の座標を加重平均
        // 動きの中心となる画素を探す
        float sum_x=0;
        float sum_y=0;
        float weight=0;
        for(int iy = 0; iy<flow.size[0]; iy++){
                const cv::Vec2f* rowptr = flow.ptr<cv::Vec2f>(iy);
                for(int ix = 0; ix<flow.size[1]; ix++){
                    float norm_sq = rowptr[ix][0]*rowptr[ix][0] + rowptr[ix][1]*rowptr[ix][1];
                    if (norm_sq<min_sq) continue;
                    float norm=std::sqrt(norm_sq);
                    float power=std::pow(norm,Config::OPTFLOW_EXP);

                    float norm=cv::norm(rowptr[ix]);
                    if (norm<Config::OPTFLOW_MOVEMIN) continue;
                    float power=std::pow(norm,Config::OPTFLOW_EXP);
                    sum_x+=ix*power;
                    sum_y+=iy*power;
                    weight+=power;
                }
            }
        
        // 注目画素を上書き
        if (weight > 0.0f){
            tgt[0]=sum_x/weight;
            tgt[1]=sum_y/weight;
            return true;
        }
    }
    // フローの大きい画素が少ない場合（動きが小さい場合）
    // 前のフレームの注目画素を引き継ぐ
    return false;
    
    
}