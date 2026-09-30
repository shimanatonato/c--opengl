// Cat.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include "src/App.h"

#ifdef _WIN32
#include <windows.h>
#endif

void __init__() {
    
}

int main()
{
	#ifdef _WIN32
		// コンソールをUTF-8表示にする
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
	#endif
	// OpenCVの内部ログ出力を消す
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	// 実行
	App app;
	app.Run();

	return 0;
}