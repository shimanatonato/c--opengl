#include "VideoCaptureWrapper.h"
#include <iostream>
#include <chrono>

// 動画読み込み時（ソースがURL文字列）
VideoCaptureWrapper::VideoCaptureWrapper(const std::string& source){
    m_capture = cv::VideoCapture(source);
    m_is_video=true;
    Initialize();
};
// カメラ読み込み時（ソースがカメラ番号）
VideoCaptureWrapper::VideoCaptureWrapper(int source){
    m_capture = cv::VideoCapture(source);
    m_is_video=false;
    Initialize();
};
// コンストラクタの共通部分
void VideoCaptureWrapper::Initialize(){
    // 動画・カメラが開けない場合
    if (!m_capture.isOpened()) {
        std::cerr << "[Error] ビデオソースを開けませんでした。" << std::endl;
        return;
    }
    // フレームが取得できない場合
    if(!m_capture.read(m_buffer)){
        std::cerr << "[Error] 最初のフレーム取得に失敗しました。" << std::endl;
        m_capture.release();  // 動画・カメラを閉じる
        return;
    }
    // 画像サイズを設定
    m_img_size = m_buffer.size();
    m_is_ready.store(true);
    // フレームの更新スレッドの開始
    m_thread=std::thread(&VideoCaptureWrapper::Update,this);
};
// デストラクタ
VideoCaptureWrapper::~VideoCaptureWrapper(){
    // フレーム更新ループ終了
    m_cancel.store(true);
    // 取得フレーム読み出し処理を終了
    m_cond.notify_all();
    // フレーム更新スレッドの終了
    if (m_thread.joinable()) {
        m_thread.join();
    }
    // 動画・カメラを閉じる
    if (m_capture.isOpened()) {
        m_capture.release();
    }
};
// 動画・カメラが開いているかどうか
bool VideoCaptureWrapper::IsOpened() const
{
    return m_capture.isOpened();
}
// フレームの更新スレッド処理
void VideoCaptureWrapper::Update(){
    // フレーム読み込み間隔の設定
    float frame_delay;
    // 動画の場合、FPSに合わせる（FPSが取得できない場合は30FPSとする）
    if(m_is_video){
        float fps = static_cast<float>(m_capture.get(cv::CAP_PROP_FPS));
        if(fps>0) frame_delay=1.0f/fps;
        else frame_delay=1.0f/30.0f;
    }
    // カメラの場合は間隔を空けず読み込み可能になり次第読み込み
    else frame_delay = 0.0;
    const auto frame_duration_ms = std::chrono::milliseconds(static_cast<int>(frame_delay*1000.0f));

    // フレーム更新ループ
    while(!m_cancel.load()){
        auto t_start = std::chrono::steady_clock::now();

        // フレームの取得
        cv::Mat temp_frame;
        // 取得できなかった場合continueで待機
        if(!m_capture.read(temp_frame)||temp_frame.empty()){
            // 動画の場合、動画の開始位置に移動（動画終了時のループ処理）
            if(m_is_video) {
                m_capture.set(cv::CAP_PROP_POS_FRAMES,0);

                // フレームレート分待機
                auto t_end = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::milli> t_elapsed = t_end-t_start;
                if(t_elapsed < frame_duration_ms) {
                    std::this_thread::sleep_for(frame_duration_ms - t_elapsed);
                }
            }
            // カメラの場合短いスリープを挟む（高速での読み込みループを防止）
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            continue;
        }

        // 取得フレームの更新
        {
            std::lock_guard<std::mutex> lock{m_mat_write};
            m_buffer=temp_frame;
            m_is_ready.store(true);  // 取得フレームを読み出し可能
        }
        m_cond.notify_one();

        // 読み込み間隔の残り時間分待機
        if(m_is_video){
            auto t_end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> t_elapsed = t_end-t_start;
            if(t_elapsed < frame_duration_ms) {
                std::this_thread::sleep_for(frame_duration_ms - t_elapsed);
            }
        }
    }
};  
// 未読の取得フレームを読み出し可能かどうか
bool VideoCaptureWrapper::IsReady() const{
    return m_is_ready.load();
};
// 取得フレームの読み出し
bool VideoCaptureWrapper::Read(cv::Mat& out_img){
    // フレームを読み込めるようになる、または動画・カメラが閉じるまで待機
    std::unique_lock<std::mutex> uniq_lk(m_mat_write);
    m_cond.wait(uniq_lk, [this]{ return m_is_ready.load()||m_cancel.load();});
    // 動画・カメラが閉じていた場合false
    if(!m_is_ready.load()||m_cancel.load()) return false;

    m_buffer.copyTo(out_img);  // 出力配列へコピー
    m_is_ready.store(false);  // フレーム出力済み
    return true;
};
// 画像サイズ出力
cv::Size VideoCaptureWrapper::GetImageSize() const{
    return m_img_size;
};