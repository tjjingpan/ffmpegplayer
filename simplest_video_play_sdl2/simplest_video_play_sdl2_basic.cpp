///**
// * 最简单的SDL2播放视频的例子（SDL2播放RGB/YUV）
// * Simplest Video Play SDL2 (SDL2 play RGB/YUV)
// *
// * 雷霄骅 Lei Xiaohua
// * leixiaohua1020@126.com
// * 中国传媒大学/数字电视技术
// * Communication University of China / Digital TV Technology
// * http://blog.csdn.net/leixiaohua1020
// *
// * 本程序使用SDL2播放RGB/YUV视频像素数据。SDL实际上是对底层绘图
// * API（Direct3D，OpenGL）的封装，使用起来明显简单于直接调用底层
// * API。
// *
// * This software plays RGB/YUV raw video data using SDL2.
// * SDL is a wrapper of low-level API (Direct3D, OpenGL).
// * Use SDL is much easier than directly call these low-level API.
// */
//
//#include <stdio.h>
//
// // 引入SDL库头文件
//extern "C"
//{
//#include "sdl2/SDL.h"
//};
//// 设置像素格式，每像素位数--每个像素占用的比特位
//const int bpp = 12;
//
//// 设置屏幕宽度和高度，以及视频像素宽度和高度
////int screen_w = 500, screen_h = 500;
////const int pixel_w = 640, pixel_h = 360;
//
//int screen_w = 500, screen_h = 500;
//const int pixel_w = 320, pixel_h = 180;
//
//// 用于存储视频帧数据的缓冲区
//unsigned char buffer[pixel_w * pixel_h * bpp / 8];
//
//int main(int argc, char* argv[])
//{
//	// 初始化SDL视频模块
//	if (SDL_Init(SDL_INIT_VIDEO)) {
//		printf("Could not initialize SDL - %s\n", SDL_GetError());
//		return -1;
//	}
//
//	// 创建SDL窗口
//	SDL_Window* screen;
//	//SDL 2.0 Support for multiple windows
//	screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//		screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
//	if (!screen) {
//		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
//		return -1;
//	}
//
//	// 创建SDL渲染器
//	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
//
//	// 设置像素格式为IYUV
//	Uint32 pixformat = 0;
//	//IYUV: Y + U + V  (3 planes)
//	//YV12: Y + V + U  (3 planes)
//	pixformat = SDL_PIXELFORMAT_IYUV;
//
//	// 创建SDL纹理
//	SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);
//
//	// 打开YUV视频文件
//	FILE* fp = NULL;
//	fp = fopen("test_yuv420p_320x180.yuv", "rb+");
//
//	if (fp == NULL) {
//		printf("cannot open this file\n");
//		return -1;
//	}
//
//	// 设置SDL矩形区域--一个简单的矩形结构
//	SDL_Rect sdlRect;
//
//	// 读取视频数据并播放
//	while (1) {
//		if (fread(buffer, 1, pixel_w * pixel_h * bpp / 8, fp) != pixel_w * pixel_h * bpp / 8) {
//			// 循环播放
//			fseek(fp, 0, SEEK_SET);
//			fread(buffer, 1, pixel_w * pixel_h * bpp / 8, fp);
//		}
//
//		// 更新SDL纹理
//		SDL_UpdateTexture(sdlTexture, NULL, buffer, pixel_w);
//
//		sdlRect.x = 10;
//		sdlRect.y = 10;
//		sdlRect.w = screen_w-20;
//		sdlRect.h = screen_h-20;
//
//		// 清除SDL渲染器
//		SDL_RenderClear(sdlRenderer);
//		// 将纹理渲染到窗口
//		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
//		// 更新渲染器显示
//		SDL_RenderPresent(sdlRenderer);
//		// 延迟40毫秒
//		SDL_Delay(40);
//	}
//	// 退出SDL
//	SDL_Quit();
//	return 0;
//}
