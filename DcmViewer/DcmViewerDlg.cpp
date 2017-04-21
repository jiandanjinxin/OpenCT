// DcmViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DcmViewer.h"
#include "DcmViewerDlg.h"
#include "afxdialogex.h"

#include <locale>
#include <direct.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\OpenCT.lib")
#else
#pragma comment(lib,"..\\Release\\OpenCT.lib")
#endif

#include "DcmFileProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenFile();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CDcmViewerDlg 对话框



CDcmViewerDlg::CDcmViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDcmViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDcmViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_axial);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_STATIC2, m_coronal);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_STATIC3, m_sagittal);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
}

BEGIN_MESSAGE_MAP(CDcmViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_OPEN_FILE, &CDcmViewerDlg::OnOpenFile)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDcmViewerDlg::OnCustomdrawSlider1)
	ON_BN_CLICKED(IDOK, &CDcmViewerDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CDcmViewerDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDcmViewerDlg::OnNMCustomdrawSlider2)
	ON_COMMAND(ID_OPEN_MARK, &CDcmViewerDlg::OnOpenMark)
END_MESSAGE_MAP()


// CDcmViewerDlg 消息处理程序

BOOL CDcmViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO:  在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT3)->SetWindowText(_T("0"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDcmViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

std::string CStringToString(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

CString GetModuleDir()
{
	HMODULE module = GetModuleHandle(0);
	TCHAR pFileName[MAX_PATH];
	GetModuleFileName(module, pFileName, MAX_PATH);

	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind(_T('\\'));
	if (nPos < 0)
		return CString("");
	else
		return csFullPath.Left(nPos);
}

void CDcmViewerDlg::OnPaint()
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


	// TODO:  在此添加额外的初始化代码
	if (image == NULL)
	{
		return;
	}
	else
	{
		//刷新picture control控件，加载打开文件的图片
		UpdateWindow();

		CRect rect1;//定义矩形类
		CRect rect2;
		CRect rect3;

		m_axial.GetClientRect(&rect1); //获得pictrue控件所在的矩形区域
		m_coronal.GetClientRect(&rect2);
		m_sagittal.GetClientRect(&rect3);

		CDC *pDc1 = m_axial.GetDC();   //获得pictrue控件的Dc
		CDC *pDc2 = m_coronal.GetDC();
		CDC *pDc3 = m_sagittal.GetDC();

		SetStretchBltMode(pDc1->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc2->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc3->m_hDC, STRETCH_HALFTONE);

		image.StretchBlt(pDc1->m_hDC, rect1, SRCCOPY);
		image_cor.StretchBlt(pDc2->m_hDC, rect2, SRCCOPY);
		image_sag.StretchBlt(pDc3->m_hDC, rect3, SRCCOPY);

		ReleaseDC(pDc1);               //释放picture控件的Dc
		ReleaseDC(pDc2);
		ReleaseDC(pDc3);
	}

	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDcmViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//重载slider范围设置函数
void CDcmViewerDlg::SetRange(int nMin, int nMax, BOOL bRedrGETaw)
{
	m_slider1.SetRange(nMin, nMax);
	m_slider2.SetRange(1, 512);
	m_slider3.SetRange(1, 512);
}

void CDcmViewerDlg::OnOpenFile()
{
	// TODO:  在此添加命令处理程序代码;

	CString FileName;
	CFileDialog dialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("All Files(*.*)|*.*||"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		FileName = dialog.GetPathName();
		filepath = dialog.GetFolderPath();
		filepath.Append(_T("\\"));

		//截取输入文件名文件夹
		std::string str = CStringToString(filepath.Left(filepath.GetLength() - 1));
		//std::vector<float> axial;
		axial.clear();
		int count = DcmFileProcess::readAllDcm(str.c_str(), axial);
		SetRange(1, count);
		
		//获取dcm图像中的第一个加载到控件中
		FileName = GetModuleDir();
		FileName.Append(_T("\\cache\\"));
		CString temp = FileName;

		FileName.Format(_T("%s%06d.bmp"), temp, 1);
		image.Load(FileName);
		FileName.Format(_T("%s%s%06d.bmp"), temp, _T("C"), 1);
		image_cor.Load(FileName);
		FileName.Format(_T("%s%s%06d.bmp"), temp, _T("S"), 1);
		image_sag.Load(FileName);

		GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1"));
		GetDlgItem(IDC_EDIT2)->SetWindowText(_T("1"));
		GetDlgItem(IDC_EDIT3)->SetWindowText(_T("1"));
		filepath = temp;
	
		//刷新picture control控件，加载打开文件的图片
		UpdateWindow();

		CRect rect1;//定义矩形类
		CRect rect2;
		CRect rect3;

		m_axial.GetClientRect(&rect1); //获得pictrue控件所在的矩形区域
		m_coronal.GetClientRect(&rect2);
		m_sagittal.GetClientRect(&rect3);

		CDC *pDc1 = m_axial.GetDC();   //获得pictrue控件的Dc
		CDC *pDc2 = m_coronal.GetDC();
		CDC *pDc3 = m_sagittal.GetDC();

		SetStretchBltMode(pDc1->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc2->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc3->m_hDC, STRETCH_HALFTONE);

		image.StretchBlt(pDc1->m_hDC, rect1, SRCCOPY);
		image_cor.StretchBlt(pDc2->m_hDC, rect2, SRCCOPY);
		image_sag.StretchBlt(pDc3->m_hDC, rect3, SRCCOPY);

		ReleaseDC(pDc1);               //释放picture控件的Dc
		ReleaseDC(pDc2);
		ReleaseDC(pDc3);
	}
	else
	{
		return;
	}
}

void CDcmViewerDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//释放图像缓存文件夹
	if (image != NULL)
	  DcmFileProcess::deleteCache();

	CDialogEx::OnOK();
}

void CDcmViewerDlg::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	//编辑框随slider更新
	CString pos;
	pos.Format(_T("%d"), m_slider1.GetPos());
	GetDlgItem(IDC_EDIT1)->SetWindowText(pos);

	if (filepath.GetLength() > 1)
	{
		CString FilePathName;
		FilePathName.Format(_T("%s%06d.bmp"), filepath ,m_slider1.GetPos());

		image.Destroy();
		image.Load(FilePathName);

		UpdateWindow();
		CRect rect;//定义矩形类
		m_axial.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_axial.GetDC();   //获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
		
	}

	*pResult = 0;
}

void CDcmViewerDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	CString pos;
	pos.Format(_T("%d"), m_slider3.GetPos());
	GetDlgItem(IDC_EDIT3)->SetWindowText(pos);

	if (filepath.GetLength() > 1)
	{
		CString FilePathName;
		FilePathName.Format(_T("%s%s%06d.bmp"), filepath, _T("C"), m_slider3.GetPos());

		image_cor.Destroy();
		image_cor.Load(FilePathName);

		UpdateWindow();
		CRect rect;//定义矩形类
		m_coronal.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_coronal.GetDC();   //获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image_cor.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
	}

	*pResult = 0;
}

void CDcmViewerDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//编辑框随slider更新
	CString pos;
	pos.Format(_T("%d"), m_slider2.GetPos());
	GetDlgItem(IDC_EDIT2)->SetWindowText(pos);

	if (filepath.GetLength() > 1)
	{
		CString FilePathName;
		FilePathName.Format(_T("%s%s%06d.bmp"), filepath, _T("S"), m_slider2.GetPos());

		image_sag.Destroy();
		image_sag.Load(FilePathName);

		UpdateWindow();
		CRect rect;//定义矩形类
		m_sagittal.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_sagittal.GetDC();   //获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image_sag.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc

	}

	*pResult = 0;
}


void CDcmViewerDlg::OnOpenMark()
{
	// TODO:  在此添加命令处理程序代码
	CString FileName;
	CFileDialog dialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("All Files(*.*)|*.*||"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		FileName = dialog.GetPathName();
		//MessageBox(FileName);
		DcmFileProcess::showPosPosition(CStringToString(FileName).c_str(), axial);
	}
	else
	{
		return;
	}
}
