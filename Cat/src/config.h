// パラメータの設定

#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

// 定数はconstexprで値が変わらないようにする
// 変数は複数ファイルで使用するのでinlineにする
namespace Config {
	// 動作モード定義
	enum class Mode : int {
		Video=0,
		Camera=1,
		Mouse=2,
		Count 
	};
	
	// デフォルト値
	inline const std::string WINDOW_NAME = "window";  // ウィンドウ名
	inline constexpr int MODE_DEFAULT = 0;  // 動作モード（0:デフォルトの動画読み込み, 1:カメラ起動, 2:マウス操作）
	inline constexpr int WIDTH_DEFAULT = 600;  // 画像サイズ
	inline constexpr int HEIGHT_DEFAULT = 400;
	inline const cv::Size IMAGE_SIZE_DEFAULT{WIDTH_DEFAULT,HEIGHT_DEFAULT};
	inline constexpr int CAMERA_NUMBER = 0;  // 起動するカメラ
	// カメラなしの場合の背景色
	inline const cv::Scalar BG_COLOR_DEFOULT{255,255,255};

	// 3D関連
	// 座標系: X + 右, Y + 上, Z + 奥
	inline const glm::vec3 CAMERA_POSITION{0.0f,0.0f,-1000.0f};  // カメラ位置
	inline constexpr float CAMERA_H = 0.0f;  // カメラの回転（ロール・ピッチ・ヨー）
	inline constexpr float CAMERA_P = 0.0f;  // カメラの回転（ロール・ピッチ・ヨー）
	inline constexpr float CAMERA_R = 0.0f;  // カメラの回転（ロール・ピッチ・ヨー）
	inline constexpr float FOCAL = 35.0f;  // 焦点距離(単位:mm)
	inline constexpr float PIXEL_WIDTH = 3.45f / 1000.0f * 4.0f; // 画素のサイズ
	inline constexpr float MONITOR_PLANE_WIDTH = 1000.0f;  // 注視する平面の幅
	inline constexpr float NEAR_PLANE = 900.0f;  // 描画範囲
    inline constexpr float FAR_PLANE  = 1100.0f;

	// モデルの設定
	inline constexpr float FINAL_HEAD_WIDTH = 150;  // モデルの幅
	inline constexpr float HEAD_SPEED = 1;  // 頭の移動速度（カーソルに追いつくまでの秒数の逆数）
	inline constexpr float EYE_SPEED = 3;  // 目の移動速度

	// モデルの詳細
	// 頭の回転の中心点（blender上の表示: [0, 0.58073, 0.65492]）
	inline const glm::vec3 HEAD_CENTER{0.0f, -0.65492f, 0.58073f};
	// 左目の回転の中心点（blender上の表示: [0.498315, -0.069373, 0.65302]）
	inline const glm::vec3 LEFT_EYE_CENTER_ABS{0.498315f,-0.65302f,-0.069373f};
	//vector<float> LEFT_EYE_CENTER = LEFT_EYE_CENTER_ABS-HEAD_CENTER;
	// 右目の回転の中心点（blender上の表示: [-0.498315, -0.069373, 0.65302]）
	inline const glm::vec3 RIGHT_EYE_CENTER_ABS{-0.498315f,-0.65302f,-0.069373f};
	//vector<float> RIGHT_EYE_CENTER = RIGHT_EYE_CENTER_ABS-HEAD_CENTER;
	inline const glm::vec3 hvect{0.0f,0.0f,-1.0f};  // 頭のデフォルトの向き

	// オプティカルフローの設定
	inline constexpr float OPTFLOW_SCALE = 4.0f;  // オプティカルフロー計算時の画像縮小率（1で元の画像サイズ）
	inline constexpr float OPTFLOW_EXP = 8.0f;  // 動きの大きさを乗算する指数
	inline constexpr float OPTFLOW_MOVEMIN = 3.0f;  // 動きの大きさを判定する最低値
	inline constexpr int OPTFLOW_PIXMIN = static_cast<int>(1000 / OPTFLOW_SCALE);  // 条件を満たす画素数の最低値

	// ゲームモードでのターゲット設定
	inline constexpr float TARGET_CLEAR_WAIT_TIME = 2.0f;  // ターゲット到達後の待ち時間
	inline constexpr float TARGET_RADIUS_MIN = 0.1f;
	inline constexpr float TARGET_RADIUS_MAX = 0.2f;
	inline const cv::Scalar TARGET_COLOR{0,0,255};
	inline const cv::Scalar TARGET_COLOR_CLEARED{0,255,255};
	inline constexpr float TARGET_ALPHA = 0.5f;
}