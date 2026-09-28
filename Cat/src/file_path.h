// 外部参照ファイルのパス
#pragma once
#include <string>

namespace FilePath{
    inline const std::string VIDEO_SOURCE = "data/video_default.mp4";  // 動画ファイルのパス
    inline const std::string TARGET_SOURCE = "data/target_default.csv";  // ターゲットファイルのパス

    inline const std::string OBJECT_HEAD_PATH = "data/cat_head.obj";  // objファイルのパス
    inline const std::string OBJECT_LEYE_PATH = "data/cat_leye.obj";  // objファイルのパス
    inline const std::string OBJECT_REYE_PATH = "data/cat_reye.obj";  // objファイルのパス
    inline const std::string MTL_DIR = "data/";  // mtlファイルの場所

    inline const std::string VERTEX_SRC_PATH="src/graphics/object.vert";
    inline const std::string FRAGMENT_SRC_PATH="src/graphics/object.frag";

    inline const std::string BG_VERTEX_SRC_PATH="src/graphics/bg.vert";
    inline const std::string BG_FRAGMENT_SRC_PATH="src/graphics/bg.frag";
}