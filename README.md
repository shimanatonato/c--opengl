# 「猫 watches you」
## プログラム概要
動画やカメラに映されたものの動きで猫の視線を誘導するARゲームです。
オプティカルフローという映像の動きの流れを計算する手法により、猫が視線を向ける注目画素を決定しています。\
猫の3DモデルはOpenGLにより描画しています。

### 操作
実行し、コマンドラインにモード（半角数字）を入力します。

**モード**：\
0：デモ用のデフォルト動画を読み込み\
1：カメラを起動してリアルタイムで撮影\
2：映像取得なしで、マウスによって操作

実行中はEscキーによって終了できます。
> 実行速度によってはEscキーの反応が悪い場合があります。Escキーで終了できない場合は、コマンドラインのウィンドウを閉じて終了してください。

### 制作目的
研究室の行事として開催された、画像プログラミングの技術習得のためのイベントがきっかけで作成しました。\
当時、研究でARを扱っていたため、既存のライブラリに頼らず描画の仕組みを深く理解した上で、より複雑なCG描画に挑戦したいと考え制作を始めました。

当初の実装では動作速度に心残りがあったこと、またその後に大学の授業でOpenGLを学んだことを機に、改良に取り組み、GPUによる描画や並列処理を実装しました。\
また、当初はPythonで作成していましたが、C++の勉強のため、処理内容をブラッシュアップしつつ移植しました。

Python版↓
https://github.com/shimanatonato/python-opengl

### アピールポイント
オプティカルフローを使うことで、手のトラッキングとは異なり、ペンなどのものにも柔軟に対応することができます。\
また、カメラのフレーム取得をメイン処理とは独立したスレッドで行うことで、カメラの読み込み待ちによるフレームレート低下を防止しています。

## 技術説明
### 開発・動作環境

- **OS**: Windows 11 (64-bit)
- **IDE**: Visual Studio 2022
- **C++ バージョン**: C++23

なお、tinyobjloaderの動作のために、load_file.cppのみC++17以下である必要があります。

### 依存ライブラリ (Dependencies)

以下のライブラリを使用しています。あらかじめビルド・配置が必要です。

- **OpenCV**: 4.12 (画像処理・入出力)
- **GLFW**: 3.4 (ウィンドウ作成)
- **GLM**: 1.0 (OpenGL 数学計算・行列計算)

以下のライブラリは同梱しています。
- **GLAD**: OpenGL 4.6 Core Profile Loader
- **tinyobjloader v2.0**: OBJファイル読み込み (MIT License)

### 1. ビルド済みバイナリをすぐに実行する場合
`bin/` フォルダ内の `Cat.exe` を実行してください。\
※ 必要な DLL (`opencv_world4120.dll`) およびアセット類（`data/`, `src/graphics/`）は `bin/` 内に同梱されています。

### 2. Visual Studio からビルドする場合
1. Visual Studio 2022 で `Cat.sln` を開きます。
2. 以下の外部ライブラリのインクルードパス・ライブラリパスを設定してください。
   - **OpenCV 4.12.0** (`opencv_world4120.lib` / `opencv_world4120d.lib`)
   - **GLFW 3.4** (`glfw3.lib`)
   - **GLM 1.0** (ヘッダーのみのためインクルードパスのみ)\
   ※ GLAD や tinyobjloader などの同梱ライブラリ（`libraries/` 配下）のパスはプロジェクト設定に含まれているため、手動設定は不要です。
3. ビルド構成を `Release` / `x64` (または `Debug` / `x64`) に設定し、ビルドを実行します。

## ファイル構成
```text
Cat
|   .gitignore
|   Cat.sln         // Visual Studioプロジェクトファイル
|   README.md       // 本ファイル
|
+---bin             // 実行用パッケージ（ダブルクリックで起動可能）
|       Cat.exe
|       opencv_world4120.dll
|       data/
|       src/graphics/
|
+---Cat
|   |   Cat.vcxproj                 // Visual Studio設定
|   |   Cat.vcxproj.filters
|   |
|   +---data                        // 3Dモデルやデフォルト動画
|   |       cat_head.obj/mtl            // 頭
|   |       cat_leye.obj/mtl            // 左目
|   |       cat_reye.obj/mtl            // 右目
|   |       video_default.mp4           // 動画モード時の表示ファイル
|   |       target_default.csv          // 動画モード時のターゲット位置
|   |
|   +---libraries               // 外部ライブラリ
|   |   |   tiny_obj_loader.h       // OBJファイル読み込み
|   |   |
|   |   \---glad                    // OpenGL 4.6 Core Profile Loader
|   |       +---include
|   |       |   +---glad
|   |       |   |       glad.h
|   |       |   |
|   |       |   \---KHR
|   |       |           khrplatform.h
|   |       |
|   |       \---src
|   |               glad.c
|   |
|   +---src                         // ソースコード
|   |   |   Cat.cpp                     // メインプログラム
|   |   |   App.h/cpp                   // 全体処理
|   |   |   config.h                    // パラメータの設定
|   |   |   file_path.h                 // 外部参照ファイルのパス
|   |   |
|   |   +---game                    // ゲーム要素関連（当たり判定、モデルの操作）
|   |   |       CatController.h/cpp     // モデルの姿勢やスケールの計算
|   |   |       TargetCircle.h/cpp      // ターゲットの生成・当たり判定
|   |   |
|   |   +---vision                  // 2D関連
|   |   |       MouseInput.h/cpp        // マウスカーソル位置の取得
|   |   |       OpticalFlowPoint.h/cpp  // オプティカルフロー処理に基づく動きの中心計算
|   |   |       VideoCaptureWrapper.h/cpp  // 動画・カメラからのフレームの取得
|   |   |
|   |   +---graphics                // 3D関連
|   |   |       bg.vert                 // 背景の頂点シェーダー
|   |   |       bg.frag                 // 背景のフラグメントシェーダー
|   |   |       object.vert             // 物体の頂点シェーダー
|   |   |       object.frag             // 物体のフラグメントシェーダー
|   |   |       Mesh.h/cpp              // オブジェクトの表示を管理するクラス
|   |   |       RenderObject.h          // オブジェクトとそのモデル行列のセットの構造体
|   |   |       Shader.h/cpp            // オブジェクト用のシェーダークラス
|   |   |       ZBufferRenderer.h/cpp   // OpenGLのZバッファー法による3D描画処理クラス
|   |   |
|   |   |
|   |   +---utils                   // その他汎用関数
|   |   |       geometry.h/cpp          // 3D処理関数
|   |   |       load_file.h/cpp         // TinyObjLoaderによるファイルの読み込み処理
|   |   |       random.h                // ランダム関数
```