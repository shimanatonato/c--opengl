// パラメータの設定

#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

// constexpr, constで値が変わらないようにする
// 複数ファイルで使用するのでinlineにする
namespace Config {
	// 動作モード定義
	enum class Mode : int {
		Video=0,  // デフォルトの動画読み込み
		Camera=1,  // カメラ起動
		Mouse=2,  // マウス操作
		Count 
	};

	inline const std::string WINDOW_NAME = "window";  // ウィンドウ名
	inline constexpr int CAMERA_NUMBER = 0;  // 起動するカメラ
	
	// カメラなしの場合のデフォルト値
	inline constexpr int WIDTH_DEFAULT = 600;  // 画像サイズ
	inline constexpr int HEIGHT_DEFAULT = 400;
	inline const cv::Size IMAGE_SIZE_DEFAULT{WIDTH_DEFAULT,HEIGHT_DEFAULT};
	inline const cv::Scalar BG_COLOR_DEFOULT{255,255,255};  // 背景色

	// =====3D関連=====
	
	// モデルの詳細
	// モデルの向きは+Xが顔の左側、+Yが下、+Zがモデルの後ろ側で作成
	inline const glm::vec3 CAT_HVECT{0.0f,0.0f,-1.0f};  // 正面のデフォルトの向き
	inline const glm::vec3 CAT_UPVECT{0.0f,-1.0f,0.0f};  // 上のデフォルトの向き
	// 頭の中心点（blender上の表示: [0, 0.58073, 0.65492]）
	inline const glm::vec3 HEAD_CENTER{0.0f, -0.65492f, 0.58073f};
	// 左目の中心点（blender上の表示: [0.498315, -0.069373, 0.65302]）
	inline const glm::vec3 LEFT_EYE_CENTER_ABS{0.498315f,-0.65302f,-0.069373f};
	inline const glm::vec3 LEFT_EYE_CENTER = LEFT_EYE_CENTER_ABS-HEAD_CENTER;
	// 右目の中心点（blender上の表示: [-0.498315, -0.069373, 0.65302]）
	inline const glm::vec3 RIGHT_EYE_CENTER_ABS{-0.498315f,-0.65302f,-0.069373f};
	inline const glm::vec3 RIGHT_EYE_CENTER = RIGHT_EYE_CENTER_ABS-HEAD_CENTER;

	// スケール設定
	inline constexpr float FINAL_HEAD_WIDTH = 150.0f;  // 頭のモデルのX軸方向の大きさ
	inline constexpr float MONITOR_PLANE_WIDTH = 1000.0f;  // 注目画素を投影する3D平面の幅

	//カメラ設定
	// カメラ座標系: +X右, +Y上, +Z手前
	// ビュー行列（外部パラメータ）
	inline const glm::vec3 CAMERA_POSITION{0.0f,0.0f,1000.0f};  // カメラ位置
	inline constexpr float CAMERA_YAW = 0.0f;  // カメラのX軸回転（ヨー）
	inline constexpr float CAMERA_PITCH = 0.0f;  // カメラのY軸回転（ピッチ）
	inline constexpr float CAMERA_ROLL = 0.0f;  // カメラのZ軸回転（ロール）
	// 投影行列（内部パラメータ）
	inline constexpr float FOCAL = 35.0f;  // 焦点距離(単位:mm)
	inline constexpr float PIXEL_WIDTH = 3.45f / 1000.0f * 4.0f; // 画素のサイズ
	inline const float NEAR_PLANE = CAMERA_POSITION.z-FINAL_HEAD_WIDTH;  // 描画範囲
    inline const float FAR_PLANE  = CAMERA_POSITION.z+FINAL_HEAD_WIDTH;

	// =====2D関連=====

	// オプティカルフローの設定
	inline constexpr float OPTFLOW_SCALE = 4.0f;  // オプティカルフロー計算時の画像縮小率（1で元の画像サイズ）
	inline constexpr float OPTFLOW_EXP = 8.0f;  // 動きの大きさを乗算する指数
	inline constexpr float OPTFLOW_MOVEMIN = 3.0f;  // 動きの大きさを判定する最低値
	inline constexpr int OPTFLOW_PIXMIN = static_cast<int>(1000 / OPTFLOW_SCALE);  // 条件を満たす画素数の最低値

	// 注目画素の処理設定
	inline constexpr float HEAD_SPEED = 1.0f;  // 頭の移動速度（カーソルに追いつくまでの秒数の逆数）
	inline constexpr float EYE_SPEED = 3.0f;  // 目の移動速度
	inline constexpr int WATCH_PT_RADIUS=10;  // 注目画素の描画時の大きさ
	inline const cv::Scalar WATCH_PT_COLOR{0,0,255};  // 注目画素の色

	// ゲームモードでのターゲット設定
	inline constexpr float TARGET_CLEAR_WAIT_TIME = 2.0f;  // ターゲット到達後の待ち時間
	inline constexpr float TARGET_RADIUS_MIN = 0.1f;  // 半径の最小値（画面の短辺に対する比率）
	inline constexpr float TARGET_RADIUS_MAX = 0.2f;  // 半径の最大値（画面の短辺に対する比率）
	inline const cv::Scalar TARGET_COLOR{0,0,255};  // 通常時の色
	inline const cv::Scalar TARGET_COLOR_CLEARED{0,255,255};  // ターゲット到達後の色
	inline constexpr float TARGET_ALPHA = 0.5f;  // 透明度
}