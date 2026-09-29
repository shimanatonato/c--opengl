#include "App.h"
#include "file_path.h"
#include <cmath>

// モード選択
void App::SelectMode(){
    // モード選択入力
    // モードに紐づけられた整数値が入力されるまでループ
    int input_mode_int=-1;
    while (true){
        std::cout << "モードを入力してください（0:デフォルトの動画を読み込み（デモ）, 1:カメラを起動, 2:マウス操作） : ";
        std::cin >> input_mode_int;
        // int型以外が入力された場合
        if(std::cin.fail()){
            std::cerr << "整数値以外を入力しないでください" << std::endl;
            // ストリームのエラーをリセット
            std::cin.clear();
            // 不正な入力ログを削除
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // 数字がモードと合わなかった場合
        if (input_mode_int<0 || static_cast<int>(Config::Mode::Count)<=input_mode_int){
            std::cerr << "モードの数値以外を入力しないでください" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // 正常入力時
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 入力バッファ内を削除
        // モード決定
        m_mode=static_cast<Config::Mode>(input_mode_int);
        break;
    }

    // フラグ設定
    // 動画・カメラ時はキャプチャを使用
    m_is_use_capture=(m_mode==Config::Mode::Video)||(m_mode==Config::Mode::Camera);
    // マウス時のみマウスを使用
    m_is_use_mouse=m_mode==Config::Mode::Mouse;
    // 動画時のみターゲット情報ファイルを読み込み
    m_is_read_target_file=m_mode==Config::Mode::Video;
    // 常にゲーム機能オン
    m_is_use_game=true;
};
// カメラパラメータ、OBJモデル、ウィンドウ、ターゲット情報の初期化
bool App::InitResources(){
    // IPPの例外発生を防止
    cv::ipp::setUseIPP(false);

    // =====動画・カメラの読み込み=====
    if (m_is_use_capture){
        if(m_mode==Config::Mode::Video){
            m_video_capture=std::make_unique<VideoCaptureWrapper>(FilePath::VIDEO_SOURCE);
        }
        else if(m_mode==Config::Mode::Camera){
            m_video_capture=std::make_unique<VideoCaptureWrapper>(Config::CAMERA_NUMBER);
        }
        if (!m_video_capture->IsOpened()) {
            std::cerr << "[Error] キャプチャが開けません" << std::endl;
            if(m_mode==Config::Mode::Video){
                std::cerr << "・file_path.h の VIDEO_SOURCE に指定されたパスに動画ファイルが存在するか確認してください。" << std::endl;
            }
            else if(m_mode==Config::Mode::Camera){
                std::cerr << "・カメラが正しくパソコンに接続されているか確認してください。" << std::endl;
                std::cerr << "・config.h の CAMERA_NUMBER が正しいか確認してください。" << std::endl;
                std::cerr << "・他のアプリがカメラを占有していないか確認してください。" << std::endl;
            }
            return false;
        }
        m_image_size=m_video_capture->GetImageSize();
    }
    else {
        // 動画・カメラなしの場合デフォルトのサイズを設定
        m_image_size=Config::IMAGE_SIZE_DEFAULT;
    }

    // =====ウィンドウ設定=====
    cv::namedWindow(m_window_name);
    if (m_is_use_mouse){
        m_mouse_input.RegisterWindow(m_window_name);
    }
    // ◆CatControllerを実装したらここでサイズ設定？
    m_focus_px={m_image_size.width/2.0f,m_image_size.height/2.0f};
    m_head_px={m_image_size.width/2.0f,m_image_size.height/2.0f};
    m_eye_px={m_image_size.width/2.0f,m_image_size.height/2.0f};

    // =====ターゲット生成クラス関連=====
    // ゲーム機能オンの場合ターゲット生成クラスの作成
    if (m_is_use_game){
        m_target_circle=Game::TargetCircle(m_image_size);
    }
    // ターゲット情報のCSVファイル読み込み
    if (m_is_read_target_file){
        m_target_circle.LoadTargetFile(FilePath::TARGET_SOURCE);
    }
    

    // =====3Dモデル関連=====
    if (!glfwInit()) {
        std::cerr << "[Error] GLFWの初期化に失敗しました" << std::endl;
        return false;
    }

    // モデルの読み込み
	std::vector<std::string> obj_paths{  // モデルのパス
		FilePath::OBJECT_HEAD_PATH,
		FilePath::OBJECT_LEYE_PATH,
		FilePath::OBJECT_REYE_PATH,
	};
	std::vector<glm::vec3> sizes;  // モデルサイズ
	m_out_vertice.reserve(obj_paths.size());
	m_out_indice.reserve(obj_paths.size());
	sizes.reserve(obj_paths.size());
    
    // 各モデルの読み込み
	for (const auto& path : obj_paths){
		std::vector<load_file::Vertex> v;
		std::vector<uint32_t> i;
		glm::vec3 size;
		load_file::load_mesh(path, FilePath::MTL_DIR, v, i, size);
        // リストに追加
		m_out_vertice.push_back(std::move(v));
		m_out_indice.push_back(std::move(i));
		sizes.push_back(size);
	}
	m_meshes.reserve(m_out_vertice.size());
	m_objs.reserve(m_meshes.size());

    // ◆後でCatControllerに移植する
	// 3Dモデルのスケールの決定
	m_scale = Config::FINAL_HEAD_WIDTH/sizes[0][0];
	m_scale_mat = geometry::scale_matrix(m_scale);
	// 頭を世界座標の中心に合わせるためのオフセット
	m_offset_head=geometry::create_view_matrix(glm::mat4(1.0f),Config::HEAD_CENTER);


    // 3D描画設定
    m_zbuf_renderer=std::make_unique<ZBufferRenderer>(m_image_size.width, m_image_size.height,false);
    
	// 投影行列（カメラ内部パラメータ）
	float cx = static_cast<float>(m_image_size.width/2.0f);
	float cy = static_cast<float>(m_image_size.height/2.0f);
	m_proj_mat = geometry::create_projection_matrix(
		Config::FOCAL/Config::PIXEL_WIDTH, Config::FOCAL/Config::PIXEL_WIDTH,
		cx, cy, 
		static_cast<float>(m_image_size.width), static_cast<float>(m_image_size.height), 
		Config::NEAR_PLANE,Config::FAR_PLANE
	);

	// ビュー行列（カメラ外部パラメータ）
	glm::mat4 camera_rot = geometry::create_rot_matrix_from_arg(Config::CAMERA_H, Config::CAMERA_P, Config::CAMERA_R);
	m_view_mat = geometry::create_view_matrix(camera_rot, Config::CAMERA_POSITION);

    m_zbuf_renderer->set_camera(m_proj_mat, m_view_mat);


    // ◆後でCatControllerに移植する

    // 目の中心にスケールや頭の回転を適用
    glm::vec3 abs_center_leye=Config::LEFT_EYE_CENTER_ABS*m_scale;
    glm::vec3 abs_center_reye=Config::RIGHT_EYE_CENTER_ABS*m_scale;
	glm::mat4 rot_head = geometry::create_rot_matrix_from_arg(0.0f, glm::radians(180.0f),0.0f);  // 頭の回転
	glm::vec3 center_leye=glm::vec3{rot_head*glm::vec4{abs_center_leye,1.0f}};
	glm::vec3 center_reye=glm::vec3{rot_head*glm::vec4{abs_center_reye,1.0f}};
	// モデル行列の作成
	glm::mat4 model_head = geometry::create_rot_matrix_around(rot_head, glm::vec3(0.0f))*m_scale_mat*m_offset_head;
	glm::mat4 model_leye = geometry::create_rot_matrix_around(glm::mat4(1.0f),center_leye)*model_head;
	glm::mat4 model_reye = geometry::create_rot_matrix_around(glm::mat4(1.0f),center_reye)*model_head;
	std::vector<glm::mat4> models{model_head,model_leye,model_reye};
	for (size_t i = 0; i < m_out_vertice.size(); ++i) {
		m_meshes.emplace_back(m_out_vertice[i], m_out_indice[i]);
		m_objs.emplace_back(&m_meshes.back(), models[i]);
	}
    return true;
};
// 入力（フレーム、マウス位置、オプティカルフロー）処理
void App::ProcessInput(){
    // 画像取得
    if(m_is_use_capture){
        // 新規取得フレームがある場合のみ読み込み
        if(m_video_capture && m_video_capture->IsReady()){
            bool is_read=m_video_capture->Read(m_showimg);
            if (!is_read||m_showimg.empty() || m_showimg.cols==0 || m_showimg.rows==0) {
                return;
            }

            // カメラの場合反転（インカメラ想定）
            if(m_mode==Config::Mode::Camera){
                cv::flip(m_showimg, m_showimg, 1);
            }
            // グレースケールに変換
            cv::cvtColor(m_showimg,m_fullimg_gray,cv::COLOR_BGR2GRAY);
            // リサイズして処理負荷軽減
            cv::resize(m_fullimg_gray,m_nextimg_gray,{0,0},1.0f/Config::OPTFLOW_SCALE,1.0f/Config::OPTFLOW_SCALE);
            // 最初のフレームは前フレーム画像の代わりに同じ画像を使用
            if(m_previmg_gray.empty()) m_nextimg_gray.copyTo(m_previmg_gray);
            // オプティカルフローから動きの中心画素を決定
            cv::Point2f out_pt;
            // フレーム全体で動きの大きさが一定以上だった場合のみ動きの中心画素を更新
            if(m_opt_flow.calcPoint(m_previmg_gray,m_nextimg_gray,out_pt)){
                m_focus_px=out_pt*Config::OPTFLOW_SCALE;  // リサイズされた座標を元に戻して代入
                // 前フレーム画像を更新
                m_previmg_gray=m_nextimg_gray.clone();;
            }
        }
    }
    // マウス入力
    else if(m_is_use_mouse){
        // 動きの中心画素をマウス位置に設定
        m_focus_px={static_cast<float>(m_mouse_input.GetX()), static_cast<float>(m_mouse_input.GetY())};
        // 背景画像を設定
        m_showimg=cv::Mat{m_image_size.height, m_image_size.width, CV_8UC3, Config::BG_COLOR_DEFOULT};
    }
    
};
// 回転計算、ターゲット判定
void App::Update(float dt){
    m_head_px=m_head_px+(m_focus_px-m_head_px)*dt*Config::HEAD_SPEED;
    m_eye_px=m_eye_px+(m_focus_px-m_eye_px)*dt*Config::EYE_SPEED;
    m_target_circle.Update(dt,m_eye_px);
    
};
// 3Dレンダリング、2D描画、imshow
void App::Render(){
    if (m_showimg.empty()) {
        return;
    }
	m_showimg=m_zbuf_renderer->draw_scene(m_showimg,m_objs);
    m_target_circle.Draw(m_showimg);
    cv::Point center(
        static_cast<int>(std::round(m_eye_px.x)),
        static_cast<int>(std::round(m_eye_px.y))
    );
    cv::circle(m_showimg,m_eye_px,m_radius_watch_pt,m_color_watch_pt,-1);

    // ↓デバッグ用 そのフレームの動きの中心画素と頭の注目画素
    cv::circle(m_showimg,cv::Point{static_cast<int>(std::round(m_focus_px.x)),
        static_cast<int>(std::round(m_focus_px.y))},m_radius_watch_pt,cv::Scalar{255,0,0} ,-1);
    cv::circle(m_showimg,cv::Point{static_cast<int>(std::round(m_head_px.x)),
        static_cast<int>(std::round(m_head_px.y))},m_radius_watch_pt,cv::Scalar{0,255,0} ,-1);
    
	// 画像の表示
	cv::imshow(m_window_name, m_showimg);
};
