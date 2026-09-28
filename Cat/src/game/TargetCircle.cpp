#include "TargetCircle.h"
#include <fstream>
#include <sstream>
#include <ostream>
#include "src/utils/random.h"

Game::TargetCircle::TargetCircle(const cv::Size& size)
    : m_image_size(size) {
}

// 到達判定
bool Game::TargetCircle::IsInside(const float x, const float y) const{
    float dx=m_current_target.x-x;
    float dy=m_current_target.y-y;
    float d=(dx*dx)+(dy*dy);  // ターゲットとマウスカーソルの距離
    return (m_current_target.r*m_current_target.r)>=d;
};

// CSVファイルの読み込み
bool Game::TargetCircle::LoadTargetFile(const std::string& source){
    m_is_read_target_file=true;
    m_target_file_ind=0;
    
    // ファイルを開く
    std::ifstream ifs(source);
    if(!ifs.is_open()) {
        std::cerr << "[Error] ターゲット情報ファイルが開けません" << std::endl;
        std::cerr << "・file_path.h の TARGET_SOURCE に指定されたパスにCSVファイルが存在するか確認してください。" << std::endl;
        return false;  
    }
    // 1行ずつ取り出し
    std::string line;
    while (std::getline(ifs,line)){
        // X,Y,Radiusの文字列から、各数値を取り出す
        std::istringstream ss(line);
        int x,y,r;
        char c1,c2;
        if (ss>> x>>c1>>y>>c2>>r){
            m_targets_list.emplace_back(x,y,r);  // リストに追加
        }
    }
    if(m_targets_list.size()>0) return true;  // 読み込み成功
    else return false;
};
// ターゲットを新しく生成
void Game::TargetCircle::SpawnCircle(){
    m_is_reached_target=false;
    // ファイルから読み込む場合
    if(m_is_read_target_file&&!m_targets_list.empty()){
        // リストから取り出し
        m_current_target=m_targets_list[m_target_file_ind];
        // インデックスを1つ進める（リスト内をループ）
        m_target_file_ind=(m_target_file_ind+1) % m_targets_list.size();
    }
    // ランダム生成の場合
    else{
        // 画像内で中心画素を決定
        int x=random::RandInt(0,m_image_size.width-1);
        int y=random::RandInt(0,m_image_size.height-1);
        // 短辺に基づき半径を決定
        int short_side=MIN(m_image_size.height,m_image_size.width);
        int r_min=static_cast<int>(m_r_min*short_side);
        int r_max=static_cast<int>(m_r_max*short_side);
        int r=random::RandInt(MIN(r_max,short_side),MAX(r_min,0));

        m_current_target=TargetInfo{x,y,r};
    }
};
// 画像にターゲットを描画
void Game::TargetCircle::Draw(cv::Mat& out_img){
    // 合成範囲の計算
    cv::Rect target_rect(  // 円のバウンディングボックス
        m_current_target.x-m_current_target.r,
        m_current_target.y-m_current_target.r,
        m_current_target.r*2+1,
        m_current_target.r*2+1
    );
    cv::Rect img_rect(0, 0, out_img.cols, out_img.rows);  // 画像内の範囲
    cv::Rect roi_rect=target_rect&img_rect;  // 画像内の描画範囲
    if (roi_rect.empty()){
        return;
    }
    // 合成範囲を切り出し
    cv::Mat roi=out_img(roi_rect);
    cv::Mat overlay=roi.clone();
    // 合成範囲のコピーにターゲットの円を描画
    cv::Point local_center(m_current_target.x-roi_rect.x, m_current_target.y-roi_rect.y);
    cv::Scalar target_color;
    if (m_is_reached_target) target_color=m_color_cleared;  // 到達済みの場合の色
    else target_color=m_color_default;  // 未到達の場合の色
    cv::circle(overlay, local_center, m_current_target.r, target_color, -1);
    // 透明度に応じて合成
    cv::addWeighted(overlay, m_alpha, roi, 1.0 - m_alpha, 0.0, roi);
};
// 状態の更新
void Game::TargetCircle::Update(const float dt, const cv::Point2f& focus_px){
    if (m_is_exist_target){
        // 未到達なら到達判定
        if (!m_is_reached_target){
            if(IsInside(focus_px.x,focus_px.y)){
                m_is_reached_target=true;
                m_reached_target_rest_time_s=Config::TARGET_CLEAR_WAIT_TIME;
            }
        }
        // 到達済みなら残りの表示時間を計算
        else{
            m_reached_target_rest_time_s-=dt;
            // 到達後 一定時間経過でターゲット削除
            if(m_reached_target_rest_time_s<=0){
                m_is_exist_target=false;
                m_is_reached_target=false;
            }
        }

    }
    // ターゲットがない場合 新規生成
    else{
        m_is_exist_target=true;
        SpawnCircle();
    }
};

// フラグの出力
bool Game::TargetCircle::IsExist() const{
    return m_is_exist_target;
};
bool Game::TargetCircle::IsReached() const{
    return m_is_reached_target;
};