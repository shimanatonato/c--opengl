// パラメータの設定

#pragma once
#include <string>
using namespace std;

namespace Config {
	// デフォルト値
	inline string VIDEO_SOURCE_DEFAULT = "source/video_default.mp4";  // 動画ファイルのパス
	inline constexpr int MODE_DEFAULT = 0;  // 動作モード（0:デフォルトの動画読み込み, 1:カメラ起動, 2:マウス操作）
	inline constexpr int WIDTH_DEFAULT = 600;  // 画像サイズ
	inline constexpr int HEIGHT_DEFAULT = 400;
	inline constexpr int CAMERA_NUMBER = 0;  // 起動するカメラ

	// 3D関連
	// 座標系: X + 右, Y + 上, Z + 奥
	inline constexpr double CAMERA_POSITION_X = 0;  // カメラ位置
	inline constexpr double CAMERA_POSITION_Y = 0;  // カメラ位置
	inline constexpr double CAMERA_POSITION_Z = -1000;  // カメラ位置
	inline constexpr double CAMERA_H = 0;  // カメラの回転（ロール・ピッチ・ヨー）
	inline constexpr double CAMERA_P = 0;  // カメラの回転（ロール・ピッチ・ヨー）
	inline constexpr double CAMERA_R = 0;  // カメラの回転（ロール・ピッチ・ヨー）
	inline constexpr double FOCAL = 35;  // 焦点距離(単位:mm)
	inline constexpr double PIXEL_WIDTH = 3.45 / 1000 * 4; // 画素のサイズ
	inline constexpr double MONITOR_PLANE_WIDTH = 1000;  // 注視する平面の幅

	// モデルの設定
	inline constexpr double FINAL_HEAD_WIDTH = 150;  // モデルの幅
	inline constexpr double HEAD_SPEED = 1;  // 頭の移動速度（カーソルに追いつくまでの秒数の逆数）
	inline constexpr double EYE_SPEED = 3;  // 目の移動速度

	// モデルの詳細
	// 頭の回転の中心点（blender上の表示: [0, 0.58073, 0.65492]）
	inline constexpr double HEAD_CENTER_X = 0;
	inline constexpr double HEAD_CENTER_Y = -0.65492;
	inline constexpr double HEAD_CENTER_Z = 0.58073;
	// 左目の回転の中心点（blender上の表示: [0.498315, -0.069373, 0.65302]）
	inline constexpr double LEFT_EYE_CENTER_ABS_X = 0.498315;
	inline constexpr double LEFT_EYE_CENTER_ABS_Y = -0.65302;
	inline constexpr double LEFT_EYE_CENTER_ABS_Z = -0.069373;
	//vector<double> LEFT_EYE_CENTER = LEFT_EYE_CENTER_ABS-HEAD_CENTER;
	// 右目の回転の中心点（blender上の表示: [-0.498315, -0.069373, 0.65302]）
	inline constexpr double RIGHT_EYE_CENTER_ABS_X = -0.498315;
	inline constexpr double RIGHT_EYE_CENTER_ABS_Y = -0.65302;
	inline constexpr double RIGHT_EYE_CENTER_ABS_Z = -0.069373;
	//vector<double> RIGHT_EYE_CENTER = RIGHT_EYE_CENTER_ABS-HEAD_CENTER;
	inline constexpr double hvect_X = 0;  // 頭のデフォルトの向き
	inline constexpr double hvect_Y = 0;  // 頭のデフォルトの向き
	inline constexpr double hvect_Z = -1;  // 頭のデフォルトの向き

	// オプティカルフローの設定
	inline constexpr double OPTFLOW_SCALE = 4;  // オプティカルフロー計算時の画像縮小率（1で元の画像サイズ）
	inline constexpr double OPTFLOW_EXP = 8;  // 動きの大きさを乗算する指数
	inline constexpr double OPTFLOW_MOVEMIN = 3;  // 動きの大きさを判定する最低値
	inline constexpr int OPTFLOW_PIXMIN = static_cast<int>(1000 / OPTFLOW_SCALE);  // 条件を満たす画素数の最低値

	// ゲームモードでのターゲット設定
	inline constexpr double TARGET_CLEAR_WAIT_TIME = 2;  // ターゲット到達後の待ち時間
	inline constexpr double TARGET_RADIUS_MIN = 0.1;
	inline constexpr double TARGET_RADIUS_MAX = 0.2;
	inline constexpr int TARGET_COLOR_R = 0;
	inline constexpr int TARGET_COLOR_G = 0;
	inline constexpr int TARGET_COLOR_B = 255;
	inline constexpr int TARGET_COLOR_CLEARED_R = 0;
	inline constexpr int TARGET_COLOR_CLEARED_G = 255;
	inline constexpr int TARGET_COLOR_CLEARED_B = 255;
	inline constexpr double TARGET_ALPHA = 0.5;
}