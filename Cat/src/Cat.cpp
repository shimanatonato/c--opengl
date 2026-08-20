// Cat.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <ranges>
#include <opencv2/opencv.hpp>
#include "src/utils/geometry.h"
#include "src/utils/load_file.h"
#include "src/graphics/ZBufferRenderer.h"
#include "src/config.h"
#include "src/file_path.h"
using namespace std;

void __init__() {
    
}

int main()
{
	if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
	
	// モデルの読み込み
	std::vector<std::string> obj_paths{
		object_head_path,
		object_leye_path,
		object_reye_path,
	};
	std::vector<std::vector<load_file::Vertex>> out_vertice;
	std::vector<std::vector<uint32_t>> out_indice;
	std::vector<glm::vec3> sizes;  // モデルの大きさ
	out_vertice.reserve(obj_paths.size());
	out_indice.reserve(obj_paths.size());
	sizes.reserve(obj_paths.size());
	for (const auto& path : obj_paths){
		std::vector<load_file::Vertex> v;
		std::vector<uint32_t> i;
		glm::vec3 size;
		load_file::load_mesh(path, mtl_dir, v, i, size);

		out_vertice.push_back(std::move(v));
		out_indice.push_back(std::move(i));
		sizes.push_back(size);
	}
	cout << sizes[0][0] <<" " << sizes[0][1] << " " << sizes[0][2] << " \n";

	// 背景画像の作成（白背景）
	int height=Config::HEIGHT_DEFAULT;
	int width=Config::WIDTH_DEFAULT;
	cv::Mat img(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

	// 3D設定
	ZBufferRenderer renderer(width, height,false);
	// 投影行列
	float cx = static_cast<float>(width/2);
	float cy = static_cast<float>(height/2);
	glm::mat4 proj = geometry::create_projection_matrix(
		static_cast<float>(Config::FOCAL/Config::PIXEL_WIDTH), static_cast<float>(Config::FOCAL/Config::PIXEL_WIDTH),
		cx, cy, 
		static_cast<float>(width), static_cast<float>(height), 
		900.0f,1100.0f
	);

	// ビュー行列
	glm::vec3 camera_pos(Config::CAMERA_POSITION_X, Config::CAMERA_POSITION_Y, Config::CAMERA_POSITION_Z);
	glm::mat4 camera_rot = geometry::create_rotmtx_from_arg(static_cast<float>(Config::CAMERA_H), static_cast<float>(Config::CAMERA_P), static_cast<float>(Config::CAMERA_R));
	glm::mat4 view = geometry::create_view_matrix(camera_rot, camera_pos);
	
	renderer.set_camera(proj, view);
	
	// モデル行列
	// 3Dモデルのスケールの決定
	float scale = static_cast<float>(Config::FINAL_HEAD_WIDTH/sizes[0][0]);
	glm::mat4 scale_mat = geometry::scale_matrix(scale);
	// 頭を世界座標の中心に合わせるためのオフセット
	glm::vec3 center_head{Config::HEAD_CENTER_X,Config::HEAD_CENTER_Y,Config::HEAD_CENTER_Z};
	glm::mat4 offset_head=geometry::create_view_matrix(glm::mat4(1.0f),center_head);
	// 目の中心にスケールや頭の回転を適用
	glm::vec3 abs_center_leye{Config::LEFT_EYE_CENTER_ABS_X,Config::LEFT_EYE_CENTER_ABS_Y,Config::LEFT_EYE_CENTER_ABS_Z};
	abs_center_leye*=scale;
	glm::vec3 abs_center_reye{Config::RIGHT_EYE_CENTER_ABS_X,Config::RIGHT_EYE_CENTER_ABS_Y,Config::RIGHT_EYE_CENTER_ABS_Z};
	abs_center_reye*=scale;
	glm::mat4 rot_head = geometry::create_rotmtx_from_arg(0.0f, glm::radians(180.0f),0.0f);  // 頭の回転
	glm::vec3 center_leye=glm::vec3{rot_head*glm::vec4{abs_center_leye,1.0f}};
	glm::vec3 center_reye=glm::vec3{rot_head*glm::vec4{abs_center_reye,1.0f}};
	// モデル行列の作成
	glm::mat4 model_head = geometry::rot_around_matrix(rot_head, glm::vec3(0.0f))*scale_mat*offset_head;
	glm::mat4 model_leye = geometry::rot_around_matrix(glm::mat4(1.0f),center_leye)*model_head;
	glm::mat4 model_reye = geometry::rot_around_matrix(glm::mat4(1.0f),center_reye)*model_head;
	std::vector<glm::mat4> models{model_head,model_leye,model_reye};

	// 3Dメッシュとモデル行列の対応付け
	std::vector<Mesh> meshes;
	meshes.reserve(out_vertice.size());
	std::vector<RenderObject> objs;
	objs.reserve(meshes.size());
	for (size_t i = 0; i < out_vertice.size(); ++i) {
		meshes.emplace_back(out_vertice[i], out_indice[i]);
		objs.emplace_back(&meshes.back(), models[i]);
	}
	// 描画
	cv::Mat drawimg(height, width, CV_8UC3);
	drawimg=renderer.draw_scene(img,objs);
	// 画像の表示
	cv::imshow("Image Display", drawimg);
	cv::waitKey(0); // キー入力待ち

	cv::destroyAllWindows(); // すべてのウィンドウを閉じる
	glfwTerminate();
	return 0;
}