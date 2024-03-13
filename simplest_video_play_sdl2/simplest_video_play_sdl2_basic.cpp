///**
// * ��򵥵�SDL2������Ƶ�����ӣ�SDL2����RGB/YUV��
// * Simplest Video Play SDL2 (SDL2 play RGB/YUV)
// *
// * ������ Lei Xiaohua
// * leixiaohua1020@126.com
// * �й���ý��ѧ/���ֵ��Ӽ���
// * Communication University of China / Digital TV Technology
// * http://blog.csdn.net/leixiaohua1020
// *
// * ������ʹ��SDL2����RGB/YUV��Ƶ�������ݡ�SDLʵ�����ǶԵײ��ͼ
// * API��Direct3D��OpenGL���ķ�װ��ʹ���������Լ���ֱ�ӵ��õײ�
// * API��
// *
// * This software plays RGB/YUV raw video data using SDL2.
// * SDL is a wrapper of low-level API (Direct3D, OpenGL).
// * Use SDL is much easier than directly call these low-level API.
// */
//
//#include <stdio.h>
//
// // ����SDL��ͷ�ļ�
//extern "C"
//{
//#include "sdl2/SDL.h"
//};
//// �������ظ�ʽ��ÿ����λ��--ÿ������ռ�õı���λ
//const int bpp = 12;
//
//// ������Ļ��Ⱥ͸߶ȣ��Լ���Ƶ���ؿ�Ⱥ͸߶�
////int screen_w = 500, screen_h = 500;
////const int pixel_w = 640, pixel_h = 360;
//
//int screen_w = 500, screen_h = 500;
//const int pixel_w = 320, pixel_h = 180;
//
//// ���ڴ洢��Ƶ֡���ݵĻ�����
//unsigned char buffer[pixel_w * pixel_h * bpp / 8];
//
//int main(int argc, char* argv[])
//{
//	// ��ʼ��SDL��Ƶģ��
//	if (SDL_Init(SDL_INIT_VIDEO)) {
//		printf("Could not initialize SDL - %s\n", SDL_GetError());
//		return -1;
//	}
//
//	// ����SDL����
//	SDL_Window* screen;
//	//SDL 2.0 Support for multiple windows
//	screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//		screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
//	if (!screen) {
//		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
//		return -1;
//	}
//
//	// ����SDL��Ⱦ��
//	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
//
//	// �������ظ�ʽΪIYUV
//	Uint32 pixformat = 0;
//	//IYUV: Y + U + V  (3 planes)
//	//YV12: Y + V + U  (3 planes)
//	pixformat = SDL_PIXELFORMAT_IYUV;
//
//	// ����SDL����
//	SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);
//
//	// ��YUV��Ƶ�ļ�
//	FILE* fp = NULL;
//	fp = fopen("test_yuv420p_320x180.yuv", "rb+");
//
//	if (fp == NULL) {
//		printf("cannot open this file\n");
//		return -1;
//	}
//
//	// ����SDL��������--һ���򵥵ľ��νṹ
//	SDL_Rect sdlRect;
//
//	// ��ȡ��Ƶ���ݲ�����
//	while (1) {
//		if (fread(buffer, 1, pixel_w * pixel_h * bpp / 8, fp) != pixel_w * pixel_h * bpp / 8) {
//			// ѭ������
//			fseek(fp, 0, SEEK_SET);
//			fread(buffer, 1, pixel_w * pixel_h * bpp / 8, fp);
//		}
//
//		// ����SDL����
//		SDL_UpdateTexture(sdlTexture, NULL, buffer, pixel_w);
//
//		sdlRect.x = 10;
//		sdlRect.y = 10;
//		sdlRect.w = screen_w-20;
//		sdlRect.h = screen_h-20;
//
//		// ���SDL��Ⱦ��
//		SDL_RenderClear(sdlRenderer);
//		// ��������Ⱦ������
//		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
//		// ������Ⱦ����ʾ
//		SDL_RenderPresent(sdlRenderer);
//		// �ӳ�40����
//		SDL_Delay(40);
//	}
//	// �˳�SDL
//	SDL_Quit();
//	return 0;
//}
