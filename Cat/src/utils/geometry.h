// 3D処理関数
#pragma once
#include <glm/glm.hpp>

namespace geometry {
	// 回転角度から回転行列を作成（入力：ヨー・ピッチ・ロール）
	glm::mat4 create_rotmtx_from_arg(float y, float p, float r);

	// center中心にrotの回転を加えた回転行列を作成
	glm::mat4 rot_around_matrix(const glm::mat4& rot, const glm::vec3& center);

	// 拡大・縮小の行列の作成
	glm::mat4 scale_matrix(float scale);

	// 投影行列の作成
	glm::mat4 create_projection_matrix(float fx, float fy, float cx, float cy, float width, float height, float near, float far);

	// ビュー行列の作成
	glm::mat4 create_view_matrix(const glm::mat4& rot, const glm::vec3& cam_pos);
}