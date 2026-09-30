#pragma once
#include <glm/glm.hpp>

// 3D処理関数
namespace geometry {
	// 回転角度から回転行列を作成（入力：ヨー・ピッチ・ロール）
	glm::mat4 create_rot_matrix_from_arg(float y, float p, float r);

	// center中心にrotの回転を加えた回転行列を作成
	glm::mat4 create_rot_matrix_around(const glm::mat4& rot, const glm::vec3& center);

	// 点から点を見つめる回転行列を作成
	glm::mat4 create_rot_matrix_look_at(const glm::vec3 from_pt,const glm::vec3 to_pt,const glm::vec3& up_vec=glm::vec3(0.0f, 1.0f, 0.0f));

	// 拡大・縮小の行列の作成
	glm::mat4 create_scale_matrix(float scale);

	// 投影行列の作成
	glm::mat4 create_projection_matrix(float fx, float fy, float cx, float cy, float width, float height, float near, float far);

	// ビュー行列の作成
	glm::mat4 create_view_matrix(const glm::mat4& rot, const glm::vec3& cam_pos);
}