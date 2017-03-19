// DcmViewerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DcmViewer.h"
#include "DcmViewerDlg.h"
#include "afxdialogex.h"

#include <locale>
#include <direct.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\OpenCV.lib")
#else
#pragma comment(lib,"..\\Release\\OpenCV.lib")
#endif

#include "DcmFileProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDcmViewerDlg �Ի���



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
	ON_BN_CLICKED(IDOK, &CDcmViewerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDcmViewerDlg ��Ϣ�������

BOOL CDcmViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


	// TODO:  �ڴ���Ӷ���ĳ�ʼ������***************************************************************
	if (image == NULL)
	{
		return;
	}
	else
	{
		//ˢ�¿��ӻ�����
		UpdateWindow();
		CRect rect;//���������
		m_axial.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_axial.GetDC();   //���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���DC
	}

	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDcmViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����slider��Χ���ú���
void CDcmViewerDlg::SetRange(int nMin, int nMax, BOOL bRedrGETaw)
{
	m_slider1.SetRange(nMin, nMax);
}


void CDcmViewerDlg::OnOpenFile()
{
	// TODO:  �ڴ���������������;

	CString FileName;
	CFileDialog dialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("Picture format(*.bmp *.jpg *jpeg)|*.bmp;*.jpg;*jpeg|All Files(*.*)|*.*||"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		FileName = dialog.GetPathName();
		filepath = dialog.GetFolderPath();
		filepath.Append(_T("\\"));

		//��ȡ�����ļ����ļ���
		std::string str = CStringToString(filepath.Left(filepath.GetLength() - 1));
		int count = DcmFileProcess::readAllDcm(str.c_str());
		SetRange(1, count);

		//��ȡdcmͼ���еĵ�һ�����ص��ؼ���
		FileName = GetModuleDir();
		FileName.Append(_T("\\cache\\"));
		CString temp = FileName;
		FileName.Format(_T("%s%06d.bmp"), temp, 1);
		image.Load(FileName);
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1"));
		filepath = temp;
		
		//ˢ��picture control�ؼ������ش��ļ���ͼƬ
		UpdateWindow();
		CRect rect;//���������
		m_axial.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_axial.GetDC();   //���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);               //�ͷ�picture�ؼ���Dc
	
	}
	else
	{
		return;
	}
}

void CDcmViewerDlg::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�༭����slider����
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
		CRect rect;//���������
		m_axial.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_axial.GetDC();   //���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
		
	}

	*pResult = 0;
}


void CDcmViewerDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ͷ�ͼ�񻺴��ļ���
	DcmFileProcess::deleteCache();


	CDialogEx::OnOK();
}
