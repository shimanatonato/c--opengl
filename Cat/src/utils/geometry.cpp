// 3D処理関数
#include "geometry.h"

namespace geometry {
    // 回転角度から回転行列を作成
    glm::mat4 create_rot_matrix_from_arg(float y, float p, float r) {
        glm::mat3 yaw(0.0f), pitch(0.0f), roll(0.0f);
        float yc = glm::cos(y); float ys = glm::sin(y);
        float pc = glm::cos(p); float ps = glm::sin(p);
        float rc = glm::cos(r); float rs = glm::sin(r);

        // X軸回転（ヨー）
        yaw[0][0] = yc;       yaw[1][0] = 0.0f;     yaw[2][0] = ys;
        yaw[0][1] = 0.0f;     yaw[1][1] = 1.0f;     yaw[2][1] = 0.0f;
        yaw[0][2] = -ys;      yaw[1][2] = 0.0f;     yaw[2][2] = yc;

        // Y軸回転（ピッチ）
        pitch[0][0] = 1.0f;   pitch[1][0] = 0.0f;   pitch[2][0] = 0.0f;
        pitch[0][1] = 0.0f;   pitch[1][1] = pc;     pitch[2][1] = -ps;
        pitch[0][2] = 0.0f;   pitch[1][2] = ps;     pitch[2][2] = pc;

        // Z軸回転（ロール）
        roll[0][0] = rc;      roll[1][0] = -rs;     roll[2][0] = 0.0f;
        roll[0][1] = rs;      roll[1][1] = rc;      roll[2][1] = 0.0f;
        roll[0][2] = 0.0f;    roll[1][2] = 0.0f;    roll[2][2] = 1.0f;
        return glm::mat4(roll * pitch * yaw);
    };

    // center中心にrotの回転を加えた回転行列を作成
    glm::mat4 create_rot_matrix_around(const glm::mat4& rot, const glm::vec3& center) {
        // centerを原点とする平行移動⇒回転⇒原点をcenterとする平行移動の順に変換
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

    // 点から点を見つめる回転行列を作成
    glm::mat4 create_rot_matrix_look_at(const glm::vec3 from_pt,const glm::vec3 to_pt,const glm::vec3& up_vec){
        // 回転後の正面方向
        glm::vec3 dir = to_pt-from_pt;

        // +Z軸（手前）
        if (glm::dot(dir,dir) < 1e-8f) {
            return glm::mat4(1.0f);
        }
        glm::vec3 forward = glm::normalize(dir);
        
        // +X軸（右）
        glm::vec3 right = glm::cross(up_vec, forward);
        if (glm::dot(right,right) < 1e-8f) {
            // forwardとupが平行な場合（真上・真下を向いたとき）
            right = glm::vec3(1.0f, 0.0f, 0.0f);
        } else {
            right = glm::normalize(right);
        }

        // 補正された+Y軸（上）
        glm::vec3 real_up = glm::cross(forward, right);

        // 回転基底から行列を作成
        glm::mat4 rot(1.0f);
        rot[0] = glm::vec4(right,    0.0f);  // X軸
        rot[1] = glm::vec4(real_up,  0.0f);  // Y軸
        rot[2] = glm::vec4(forward,  0.0f);  // Z軸
        
        return rot;
    };

    // 拡大・縮小の行列の作成
    glm::mat4 create_scale_matrix(float scale) {
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
        glm::vec3 cam_pos_rotated = -glm::mat3(rot)*cam_pos;

        glm::mat4 view_mat = rot;
        view_mat[3] = glm::vec4(cam_pos_rotated, 1.0f);

        return view_mat;
    };
}