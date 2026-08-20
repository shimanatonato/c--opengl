// 3D処理関数
#include "geometry.h"

namespace geometry {
    // 回転角度から回転行列を作成
    glm::mat4 create_rotmtx_from_arg(float y, float p, float r) {
        glm::mat3 yaw(0.0f), pitch(0.0f), roll(0.0f);
        float yc = glm::cos(y); float ys = glm::sin(y);
        float pc = glm::cos(p); float ps = glm::sin(p);
        float rc = glm::cos(r); float rs = glm::sin(r);

        yaw[0][0] = yc;       yaw[1][0] = 0.0f;     yaw[2][0] = ys;
        yaw[0][1] = 0.0f;     yaw[1][1] = 1.0f;     yaw[2][1] = 0.0f;
        yaw[0][2] = -ys;      yaw[1][2] = 0.0f;     yaw[2][2] = yc;

        pitch[0][0] = 1.0f;   pitch[1][0] = 0.0f;   pitch[2][0] = 0.0f;
        pitch[0][1] = 0.0f;   pitch[1][1] = pc;     pitch[2][1] = -ps;
        pitch[0][2] = 0.0f;   pitch[1][2] = ps;     pitch[2][2] = pc;

        roll[0][0] = rc;      roll[1][0] = -rs;     roll[2][0] = 0.0f;
        roll[0][1] = rs;      roll[1][1] = rc;      roll[2][1] = 0.0f;
        roll[0][2] = 0.0f;    roll[1][2] = 0.0f;    roll[2][2] = 1.0f;
        return glm::mat4(roll * pitch * yaw);
    };

    // center中心にrotの回転を加えた回転行列を作成
    glm::mat4 rot_around_matrix(const glm::mat4& rot, const glm::vec3& center) {
        glm::mat4 t_plus(1.0f);  // 回転後の平行移動
        glm::mat4 t_minus(1.0f);  // 回転前の平行移動

        t_plus[3][0] = center.x;
        t_plus[3][1] = center.y;
        t_plus[3][2] = center.z;

        t_minus[3][0] = -center.x;
        t_minus[3][1] = -center.y;
        t_minus[3][2] = -center.z;

        return t_plus * rot * t_minus;
    };

    // 拡大・縮小の行列の作成
    glm::mat4 scale_matrix(float scale) {
        glm::mat4 mat(scale);
        mat[3][3] = 1.0f;
        return mat;
    };

    // 投影行列の作成
    glm::mat4 create_projection_matrix(float fx, float fy, float cx, float cy, float width, float height, float near, float far) {
        glm::mat4 mat(0.0f);
        mat[0][0] = 2*fx/width; mat[2][0] = 2 * cx / width - 1;
        mat[1][1] = 2*fy/height; mat[2][1] = 1 - 2 * cy / height;
        mat[2][2] = -(far + near) / (far - near); mat[3][2] = -2 * far * near / (far - near);
        mat[2][3] = -1.0f;
        return mat;
    };

    // ビュー行列の作成
    glm::mat4 create_view_matrix(const glm::mat4& rot, const glm::vec3& cam_pos) {
        glm::mat4 view_rot(1.0f);
        for (int c = 0; c < 3; ++c) {
            view_rot[c][0] = rot[c][0];
            view_rot[c][1] = -rot[c][1];
            view_rot[c][2] = -rot[c][2];
        }
        glm::mat4 view_trans(1.0f);
        view_trans[3][0] = -cam_pos.x;
        view_trans[3][1] = -cam_pos.y;
        view_trans[3][2] = -cam_pos.z;

        return view_rot * view_trans;
    };
}