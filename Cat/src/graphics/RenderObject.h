// オブジェクトとそのモデル行列のセットの構造体
#pragma once
#include "Mesh.h"

struct RenderObject {
    const Mesh* mesh;       // 描画するメッシュへのポインタ（メッシュはMain側で所有）
    glm::mat4 model_matrix; // そのメッシュのモデル行列
};
