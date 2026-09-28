#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class VideoCaptureWrapper
{
    private:
        cv::Size m_img_size;
        std::thread m_thread;  // フレームを更新するスレッド
        cv::VideoCapture m_capture;
        bool m_is_video;  // 動画かカメラか
        cv::Mat m_buffer;  // フレーム画像
        std::atomic<bool> m_is_ready = false;  // 未読の取得フレームを読み出し可能かどうか
        std::mutex m_mtx_write;  // m_buffer, m_readyの読み書きの管理
        std::condition_variable m_cond;  // 読み出し可能になるまでの待機処理の管理
        std::atomic<bool> m_cancel{false};  // 終了判定
        // フレームの更新
        void Update();
        // コンストラクタの共通部分（メンバ変数の設定）
        void Initialize();
    public:
        // 動画読み込み時
        VideoCaptureWrapper(const std::string& source);
        // カメラ読み込み時
        VideoCaptureWrapper(int source);
        ~VideoCaptureWrapper();
        // 未読の取得フレームを読み出し可能かどうか
        bool IsReady() const;
        // 動画・カメラを開けているか
        bool IsOpened() const;
        // 取得フレームの読み出し
        bool Read(cv::Mat& out_img);
        // 画像サイズ出力
        cv::Size GetImageSize() const;
};

