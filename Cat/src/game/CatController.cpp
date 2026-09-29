#include "CatController.h"
#include "utils/geometry.h"

// 注目している2次元座標を3次元座標に変換
glm::vec3 CatController::PixelTo3D(const cv::Point2f& p2d, const cv::Size& img_size) const{
    float monitor_plane_pix_size=Config::MONITOR_PLANE_WIDTH/static_cast<float>(img_size.width);

    float x_3d=(p2d.x-static_cast<float>(img_size.width)*0.5f)*monitor_plane_pix_size;
    float y_3d=(p2d.y-static_cast<float>(img_size.height)*0.5f)*monitor_plane_pix_size;

    return glm::vec3{x_3d,y_3d,Config::CAMERA_POSITION.z};
};

// スケーリング設定
void CatController::setScale(const float scale){
    m_scale=scale;
    m_scale_mat=geometry::create_scale_matrix(scale);
    m_center_leye=Config::LEFT_EYE_CENTER_ABS*scale;
    m_center_reye=Config::RIGHT_EYE_CENTER_ABS*scale;
};

// モデル行列の計算
// 各オブジェクトの注目位置は(座標/画像サイズ)で与える
CatPose CatController::CalcCatPose(const cv::Point2f& head_px, const cv::Point2f& eye_px, const cv::Size& img_size) const{
    // 注目位置を3次元座標に変換
    glm::vec3 head_p3d=PixelTo3D(head_px,img_size);
    glm::vec3 eye_p3d=PixelTo3D(eye_px,img_size);

    // 頭の計算
    glm::mat4 head_rot=geometry::create_rot_matrix_look_at(glm::vec3{0.0f},head_p3d);

    // 目の計算
    // 頭の回転を考慮し中心位置の世界座標を計算
    glm::vec3 center_leye_world=glm::vec3(head_rot*glm::vec4(m_center_leye, 1.0f));
    glm::vec3 center_reye_world=glm::vec3(head_rot*glm::vec4(m_center_reye, 1.0f));
    // 回転計算
    glm::mat4 leye_rot=geometry::create_rot_matrix_look_at(center_leye_world,eye_p3d);
    glm::mat4 reye_rot=geometry::create_rot_matrix_look_at(center_reye_world,eye_p3d);

    // 姿勢計算
    CatPose pose;
    pose.head=head_rot*m_scale_mat*m_offset_center_mat;
    pose.left_eye=geometry::create_rot_matrix_around(leye_rot,center_leye_world)*pose.head;
    pose.right_eye=geometry::create_rot_matrix_around(reye_rot,center_reye_world)*pose.head;

    return pose;
};