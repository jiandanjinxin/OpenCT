
// DcmViewerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDcmViewerDlg 对话框
class CDcmViewerDlg : public CDialogEx
{
// 构造
public:
	CDcmViewerDlg(CWnd* pParent = NULL);	// 标准构造函数
	//~CDcmViewerDlg();
// 对话框数据
	enum { IDD = IDD_DCMVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void GetRange(int &nMin, int &nMax) const;
	void SetRange(int nMin, int nMax, BOOL bRedrGETaw = FALSE);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenFile();
	afx_msg void OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);

	CImage image;        //创建图片类
	CString filepath;    //打开文件的文件夹目录

	CStatic m_axial;
	CSliderCtrl m_slider1;
	CEdit m_edit1;
	afx_msg void OnBnClickedOk();
};
