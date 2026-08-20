# 「猫 watches you」
## プログラム概要
OpenGLにより3Dモデルを表示するプログラムです。
Pythonで作成したものをC++に移植中です。
最終的に移植が完了すればARゲームになります。

Python版のARゲーム↓
https://github.com/shimanatonato/python-opengl

### 制作目的
研究室の行事として開催された、画像プログラミングの技術習得のためのイベントがきっかけで作成しました。\
当時、研究でARを扱っていたため、既存のライブラリに頼らず描画の仕組みを深く理解した上で、より複雑なCG描画に挑戦したいと考え制作を始めました。

当初の実装では動作速度に心残りがあったこと、またその後に大学の授業でOpenGLを学んだことを機に、自主的に改良に取り組みました。

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

## ファイル構成
```text
Cat
|   .gitignore
|   Cat.sln         // Visual Studioプロジェクトファイル
|   README.md       // 本ファイル
|
+---Cat
|   |   Cat.vcxproj                 // Visual Studio設定
|   |   Cat.vcxproj.filters
|   |
|   +---data                        // 3Dモデルやデフォルト動画
|   |       cat_head.obj/mtl            // 頭
|   |       cat_leye.obj/mtl            // 左目
|   |       cat_reye.obj/mtl            // 右目
|   |
|   +---src                         // ソースコード
|   |   |   Cat.cpp                     // メインプログラム
|   |   |   config.h                    // パラメータの設定
|   |   |   file_path.h                 // 外部参照ファイルのパス
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
|   |   +---libraries               // 外部ライブラリ
|   |   |   |   tiny_obj_loader.h       // OBJファイル読み込み
|   |   |   |
|   |   |   \---glad                    // OpenGL 4.6 Core Profile Loader
|   |   |       +---include
|   |   |       |   +---glad
|   |   |       |   |       glad.h
|   |   |       |   |
|   |   |       |   \---KHR
|   |   |       |           khrplatform.h
|   |   |       |
|   |   |       \---src
|   |   |               glad.c
|   |   |
|   |   +---utils                   // その他汎用関数
|   |   |       geometry.h/cpp          // 3D処理関数
|   |   |       load_file.h/cpp         // TinyObjLoaderによるファイルの読み込み処理
```