///**
// * 最简单的基于FFmpeg的解码器
// * Simplest FFmpeg Decoder
// *
// * 雷霄骅 Lei Xiaohua
// * leixiaohua1020@126.com
// * 中国传媒大学/数字电视技术
// * Communication University of China / Digital TV Technology
// * http://blog.csdn.net/leixiaohua1020
// *
// * 本程序实现了视频文件的解码(支持HEVC，H.264，MPEG2等)。
// * 是最简单的FFmpeg视频解码方面的教程。
// * 通过学习本例子可以了解FFmpeg的解码流程。
// * This software is a simplest video decoder based on FFmpeg.
// * Suitable for beginner of FFmpeg.
// *
// */
//
//#include <stdio.h>
//
//#define __STDC_CONSTANT_MACROS
//
//extern "C"
//{
//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
//#include "libswscale/swscale.h"
//};
//
//
//int main(int argc, char* argv[])
//{
//	AVFormatContext* pFormatCtx;		//音视频封装格式的上下文
//	int				i, videoindex;
//	AVCodecContext* pCodecCtx;			//编码器上下文结构体，保存了视频（音频）编解码相关信息
//	AVCodec* pCodec;					//解码器，HEVC，H.264，MPEG2等
//	AVFrame* pFrame, * pFrameYUV;		//该结构描述解码的（原始的）音频或视频数据。
//	uint8_t* out_buffer;
//	AVPacket* packet;					//此结构存储压缩数据				
//	int y_size;
//	int ret, got_picture;				//存储解码的返回值
//	struct SwsContext* img_convert_ctx;
//	//输入文件路径
//	//char filepath[] = "cuc_ieschool.flv";
//	char filepath[] = "Titanic.ts";
//
//	int frame_cnt;
//
//	av_register_all();							// 注册所有的文件格式和编解码器
//	avformat_network_init();					// 初始化网络流格式，使用网络流时需要调用		
//	pFormatCtx = avformat_alloc_context();		// 初始化一个封装格式上下文
//
//	// 打开输入的视频文件
//	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {
//		printf("Couldn't open input stream.\n");
//		return -1;
//	}
//
//	// 获取视频文件的流信息
//	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
//		printf("Couldn't find stream information.\n");
//		return -1;
//	}
//
//	//获取视频的序号，如果没有找到视频序号，打印结果，程序退出
//	videoindex = -1;
//	for (i = 0; i < pFormatCtx->nb_streams; i++)
//		
//		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
//			videoindex = i;
//			break;
//		}
//	if (videoindex == -1) {
//		printf("Didn't find a video stream.\n");
//		return -1;
//	}
//
//	pCodecCtx = pFormatCtx->streams[videoindex]->codec;// 获取视频流的解码器上下文
//	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);// 根据编码器ID查找解码器
//	if (pCodec == NULL) {
//		printf("Codec not found.\n");
//		return -1;
//	}
//	// 打开解码器
//	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
//		printf("Could not open codec.\n");
//		return -1;
//	}
//
//	printf("视频的时长:%d\n", pFormatCtx->duration);
//	printf("封装格式名%s\n", pFormatCtx->iformat->name);
//	printf("封装长格式名%s\n", pFormatCtx->iformat->long_name);
//	printf("视频的宽高%d*%d", pFormatCtx->streams[videoindex]->codec->width, pFormatCtx->streams[videoindex]->codec->height);
//
//	FILE* fp = fopen("info.txt", "wb+");
//	fprintf(fp, "视频的时长:%d\n", pFormatCtx->duration);
//	fprintf(fp, "封装格式名%s\n", pFormatCtx->iformat->name);
//	fprintf(fp, "封装长格式名%s\n", pFormatCtx->iformat->long_name);
//	fprintf(fp, "视频的宽高%d*%d", pFormatCtx->streams[videoindex]->codec->width, pFormatCtx->streams[videoindex]->codec->height);
//	fclose(fp);
//
//
//	/*
//	 * 在此处添加输出视频信息的代码
//	 * 取自于pFormatCtx，使用fprintf()
//	 */
//	pFrame = av_frame_alloc();		// 初始化帧结构体
//	pFrameYUV = av_frame_alloc();	// 初始化YUV格式帧结构体
//	// 获取YUV缓冲区大小
//	out_buffer = (uint8_t*)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
//	// 填充YUV帧数据
//	avpicture_fill((AVPicture*)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
//	// 分配存储packet的内存
//	packet = (AVPacket*)av_malloc(sizeof(AVPacket));
//	//Output Info-----------------------------
//	printf("--------------- File Information ----------------\n");
//	av_dump_format(pFormatCtx, 0, filepath, 0);// 打印文件信息
//	printf("-------------------------------------------------\n");
//
//	// 初始化图像转换上下文
//	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
//		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
//
//	FILE* fp_264 = fopen("test264.h264", "wb+");
//	FILE* fp_yuv = fopen("testyuv.yuv", "wb+");
//	frame_cnt = 0;
//	while (av_read_frame(pFormatCtx, packet) >= 0) {// 循环读取帧数据
//		if (packet->stream_index == videoindex) {
//			/*
//			 * 在此处添加输出H264码流的代码
//			 * 取自于packet，使用fwrite()
//			 */
//			fwrite(packet->data, 1, packet->size, fp_264);
//
//			// 解码视频帧
//			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
//			if (ret < 0) {
//				printf("Decode Error.\n");
//				return -1;
//			}
//			if (got_picture) {
//				sws_scale(img_convert_ctx,
//					(const uint8_t* const*)pFrame->data,
//					pFrame->linesize,
//					0,
//					pCodecCtx->height,
//					pFrameYUV->data,
//					pFrameYUV->linesize);// 转换为YUV格式
//				printf("Decoded frame index: %d\n", frame_cnt);
//
//				/*
//				 * 在此处添加输出YUV的代码
//				 * 取自于pFrameYUV，使用fwrite()
//				 */
//				fwrite(pFrameYUV->data[0], 1, pCodecCtx->width * pCodecCtx->height, fp_yuv);
//				fwrite(pFrameYUV->data[1], 1, pCodecCtx->width * pCodecCtx->height / 4, fp_yuv);
//				fwrite(pFrameYUV->data[2], 1, pCodecCtx->width * pCodecCtx->height / 4, fp_yuv);
//				frame_cnt++;
//
//			}
//		}
//		av_free_packet(packet);
//	}
//
//	fclose(fp_264);
//	fclose(fp_yuv);
//
//	sws_freeContext(img_convert_ctx);		// 释放图像转换上下文
//
//	av_frame_free(&pFrameYUV);				// 释放YUV格式帧结构体
//	av_frame_free(&pFrame);					// 释放帧结构体
//	avcodec_close(pCodecCtx);				//关闭解码器
//	avformat_close_input(&pFormatCtx);		// 关闭输入流
//
//	return 0;
//}
//
