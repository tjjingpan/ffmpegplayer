
// testplayermfcDlg.h: 头文件
//

#pragma once


// CtestplayermfcDlg 对话框
class CtestplayermfcDlg : public CDialogEx
{
// 构造
public:
	CtestplayermfcDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTPLAYERMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPlayer();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedFiledialog();
	CEdit m_url;
	CStatic m_videoControl;
//	afx_msg void OnBnClickedResume();
	CButton btn_player;
	CButton btn_pause;
	afx_msg void OnStnClickedScreen();
};
