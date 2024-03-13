
// testplayermfcDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "testplayermfc.h"
#include "testplayermfcDlg.h"
#include "afxdialogex.h"

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL2/SDL.h"
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestplayermfcDlg 对话框
CtestplayermfcDlg::CtestplayermfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTPLAYERMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestplayermfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_URL, m_url);
	DDX_Control(pDX, IDC_SCREEN, m_videoControl);
	DDX_Control(pDX, IDC_PLAYER, btn_player);
	DDX_Control(pDX, IDC_PAUSE, btn_pause);
}

BEGIN_MESSAGE_MAP(CtestplayermfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CtestplayermfcDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_PLAYER, &CtestplayermfcDlg::OnBnClickedPlayer)
	ON_BN_CLICKED(IDC_PAUSE, &CtestplayermfcDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, &CtestplayermfcDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_ABOUT, &CtestplayermfcDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_FILEDIALOG, &CtestplayermfcDlg::OnBnClickedFiledialog)
//	ON_BN_CLICKED(IDC_RESUME, &CtestplayermfcDlg::OnBnClickedResume)
ON_STN_CLICKED(IDC_SCREEN, &CtestplayermfcDlg::OnStnClickedScreen)
END_MESSAGE_MAP()


// 声明一些全局变量，用于控制播放状态
bool g_bPaused = false;
bool g_bStopped = false;


 UINT ffmpegplayer(LPVOID lpParam)
{
		CtestplayermfcDlg* dlg = (CtestplayermfcDlg*)lpParam;
		g_bPaused = false;
		g_bStopped = false;

		if (!g_bStopped)//没有停止
		{
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(false);//按钮禁用
			dlg->GetDlgItem(IDC_PAUSE)->EnableWindow(true);//按钮禁用
		}
		else {//停止
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(false);//按钮启用
			dlg->GetDlgItem(IDC_PAUSE)->EnableWindow(false);//按钮禁用
		}

		AVFormatContext* pFormatCtx;		//音视频封装格式的上下文
		int				i;
		int videoStream = -1;
		AVCodecContext* pCodecCtx;			//编码器上下文结构体，保存了视频（音频）编解码相关信息
		AVCodec* pCodec;					//解码器，HEVC，H.264，MPEG2等
		AVFrame* pFrame, * pFrameYUV;		//该结构描述解码的（原始的）音频或视频数据。
		uint8_t* out_buffer;
		AVPacket* packet;					//此结构存储压缩数据				
		int y_size;
		int ret, got_picture;				//存储解码的返回值
		struct SwsContext* img_convert_ctx;
		//输入文件路径
		//char filepath[] = "cuc_ieschool.flv";

		
		//文件路径
		
		char filepath[250] = { 0 };

		GetWindowTextA(dlg->m_url, (LPSTR)filepath, 250);
		int frame_cnt;						//视频帧个数


		av_register_all();							// 注册所有的文件格式和编解码器
		avformat_network_init();					// 初始化网络流格式，使用网络流时需要调用		
		pFormatCtx = avformat_alloc_context();		// 初始化一个封装格式上下文

		if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0)
		{
			AfxMessageBox(_T("无法打开视频文件"));
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(true);//按钮启用
			return 0;
		}

		// 获取视频文件的流信息
		if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
			AfxMessageBox(_T("无法获取视频流信息"));
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(true);//按钮启用
			return 0;
		}



		//获取视频的序号，如果没有找到视频序号，打印结果，程序退出

		for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
		{
			if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				videoStream = i;
				break;
			}
		}
		if (videoStream == -1)
		{
			AfxMessageBox(_T("无法找到视频流"));
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(true);//按钮启用
			return 0;
		}

		// 获取视频解码器
		pCodecCtx = pFormatCtx->streams[videoStream]->codec;// 获取视频流的解码器上下文
		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);// 根据编码器ID查找解码器
		if (pCodec == NULL) {
			AfxMessageBox(_T("无法找到视频解码器"));
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(true);//按钮启用
			return 0;
		}

		// 打开解码器
		if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
			AfxMessageBox(_T("无法打开解码器"));
			dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(true);//按钮启用
			return 0;
		}

		/*
		* 在此处添加输出视频信息的代码
		* 取自于pFormatCtx，使用fprintf()
		*/
		pFrame = av_frame_alloc();		// 初始化帧结构体
		pFrameYUV = av_frame_alloc();	// 初始化YUV格式帧结构体
		// 获取YUV缓冲区大小
		out_buffer = (uint8_t*)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
		// 填充YUV帧数据
		avpicture_fill((AVPicture*)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
		// 分配存储packet的内存
		packet = (AVPacket*)av_malloc(sizeof(AVPacket));

		// 初始化SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
		{
			AfxMessageBox(_T("无法初始化SDL"));
			return 0;
		}

		// 创建SDL窗口
		SDL_Window* window = SDL_CreateWindowFrom(dlg->GetDlgItem(IDC_SCREEN)->GetSafeHwnd()); // 将 SDL 窗口绑定到 Picture Control 控件上
		if (!window)
		{
			AfxMessageBox(_T("无法创建SDL窗口"));
			return 0;
		}
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

		img_convert_ctx = sws_getContext(
			pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
			pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P,
			SWS_BICUBIC, NULL,
			NULL, NULL);



		// 解码并渲染视频
		while (av_read_frame(pFormatCtx, packet) >= 0)
		{
			if (packet->stream_index == videoStream)
			{

				// 处理暂停状态
				while (g_bPaused)
				{
					SDL_Delay(100); // 等待100ms
				}

				int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
				if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
				{
					AfxMessageBox(_T("解码错误"));
					break;
				}
				if (got_picture >= 0)
				{
					sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
					SDL_UpdateYUVTexture(texture, NULL, pFrame->data[0], pFrame->linesize[0], pFrame->data[1], pFrame->linesize[1], pFrame->data[2], pFrame->linesize[2]);
					SDL_RenderClear(renderer);
					SDL_RenderCopy(renderer, texture, NULL, NULL);
					SDL_RenderPresent(renderer);
					SDL_Delay(40);
				}

				//处理停止
				if (g_bStopped)
				{	
					g_bStopped = false;
					break;
				}		
			}
			//av_free_packet(packet);
			av_packet_unref(packet);
		}

		sws_freeContext(img_convert_ctx);

		SDL_Quit();
		//FIX Small Bug
		//SDL Hide Window When it finished
		//dlg->GetDlgItem(IDC_SCREEN)->ShowWindow(SW_SHOWNORMAL);
		//--------------
		av_frame_free(&pFrameYUV);
		av_frame_free(&pFrame);
		avcodec_close(pCodecCtx);
		avformat_close_input(&pFormatCtx);
		dlg->GetDlgItem(IDC_PLAYER)->EnableWindow(true);//播放完启用播放按钮
		return 0;
}








// CtestplayermfcDlg 消息处理程序

BOOL CtestplayermfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestplayermfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestplayermfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestplayermfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtestplayermfcDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CtestplayermfcDlg::OnBnClickedPlayer()
{
	CString str1;
	m_url.GetWindowText(str1);
	if (str1.IsEmpty())
	{
		AfxMessageBox("请输入文件路径!");
		return;
	}

	AfxBeginThread(ffmpegplayer, this);//开启线程
	if (g_bStopped)
	{
		btn_player.EnableWindow(true);
		btn_pause.EnableWindow(true);
	}
	else {
		btn_player.EnableWindow(false);
		btn_pause.EnableWindow(false);
	}
}


// 暂停播放函数
void PausePlayback()
{
	g_bPaused = true;
}

// 停止播放函数
void StopPlayback()
{
	g_bStopped = true;
}

void CtestplayermfcDlg::OnBnClickedPause()
{
	btn_player.EnableWindow(false);
	if (g_bPaused)
	{	
		btn_pause.SetWindowTextA("暂停");
		g_bPaused = false;
	}
	else {
		g_bPaused = true;
		btn_pause.SetWindowTextA("恢复");
	}
}

void CtestplayermfcDlg::OnBnClickedStop()
{
	StopPlayback();
	btn_player.EnableWindow(true);
}


void CtestplayermfcDlg::OnBnClickedAbout()
{
	CAboutDlg dig1;
	dig1.DoModal();//模式对话框
}


void CtestplayermfcDlg::OnBnClickedFiledialog()
{
	/*CString str1;
	str1.Format("%s", avcodec_configuration());
	AfxMessageBox(str1);*/

	CString FilePathName;
	CFileDialog dlg(TRUE, NULL, NULL, NULL, NULL);//TRUE为OPEN对话框
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
		m_url.SetWindowText(FilePathName);
	}
}

void CtestplayermfcDlg::OnStnClickedScreen()
{
	// TODO: 在此添加控件通知处理程序代码
}
