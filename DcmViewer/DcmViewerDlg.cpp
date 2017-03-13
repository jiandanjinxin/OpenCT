
// DcmViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DcmViewer.h"
#include "DcmViewerDlg.h"
#include "afxdialogex.h"

#include <locale>

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
}

BEGIN_MESSAGE_MAP(CDcmViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_OPEN_FILE, &CDcmViewerDlg::OnOpenFile)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDcmViewerDlg::OnCustomdrawSlider1)
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
	SetRange(1, 600);

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


	// TODO:  在此添加额外的初始化代码***************************************************************
	if (image == NULL)
	{
		return;
	}
	else
	{
		//刷新可视化区域
		UpdateWindow();
		int height, width;
		CRect rect;//定义矩形类
		height = image.GetHeight();
		width = image.GetWidth();
		m_axial.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_axial.GetDC();   //获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的DC
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
}


void CDcmViewerDlg::OnOpenFile()
{
	// TODO:  在此添加命令处理程序代码;

	CString FilePathName;
	CFileDialog dialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("Picture format(*.bmp *.jpg *jpeg)|*.bmp;*.jpg;*jpeg|All Files(*.*)|*.*||"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		FilePathName = dialog.GetPathName();
		image.Load(FilePathName);
		filepath = FilePathName.Left(FilePathName.GetLength() - 10);
		//刷新picture control控件，加载打开文件的图片
		UpdateWindow();
		int height, width;
		CRect rect;//定义矩形类
		height = image.GetHeight();
		width = image.GetWidth();
		m_axial.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_axial.GetDC();   //获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc

		//截取输入文件名文件夹
	}
	else
	{
		return;
	}
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
		int height, width;
		CRect rect;//定义矩形类
		height = image.GetHeight();
		width = image.GetWidth();
		m_axial.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_axial.GetDC();   //获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
		
	}

	*pResult = 0;
}
