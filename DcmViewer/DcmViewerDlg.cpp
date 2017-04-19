// DcmViewerDlg.cpp : ʵ���ļ�
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
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT3)->SetWindowText(_T("0"));

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


	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	if (image == NULL)
	{
		return;
	}
	else
	{
		//ˢ��picture control�ؼ������ش��ļ���ͼƬ
		UpdateWindow();

		CRect rect1;//���������
		CRect rect2;
		CRect rect3;

		m_axial.GetClientRect(&rect1); //���pictrue�ؼ����ڵľ�������
		m_coronal.GetClientRect(&rect2);
		m_sagittal.GetClientRect(&rect3);

		CDC *pDc1 = m_axial.GetDC();   //���pictrue�ؼ���Dc
		CDC *pDc2 = m_coronal.GetDC();
		CDC *pDc3 = m_sagittal.GetDC();

		SetStretchBltMode(pDc1->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc2->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc3->m_hDC, STRETCH_HALFTONE);

		image.StretchBlt(pDc1->m_hDC, rect1, SRCCOPY);
		image_cor.StretchBlt(pDc2->m_hDC, rect2, SRCCOPY);
		image_sag.StretchBlt(pDc3->m_hDC, rect3, SRCCOPY);

		ReleaseDC(pDc1);               //�ͷ�picture�ؼ���Dc
		ReleaseDC(pDc2);
		ReleaseDC(pDc3);
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
	m_slider2.SetRange(1, 512);
	m_slider3.SetRange(1, 512);
}

void CDcmViewerDlg::OnOpenFile()
{
	// TODO:  �ڴ���������������;

	CString FileName;
	CFileDialog dialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("All Files(*.*)|*.*||"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		FileName = dialog.GetPathName();
		filepath = dialog.GetFolderPath();
		filepath.Append(_T("\\"));

		//��ȡ�����ļ����ļ���
		std::string str = CStringToString(filepath.Left(filepath.GetLength() - 1));
		//std::vector<float> axial;
		axial.clear();
		int count = DcmFileProcess::readAllDcm(str.c_str(), axial);
		SetRange(1, count);
		
		//��ȡdcmͼ���еĵ�һ�����ص��ؼ���
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
	
		//ˢ��picture control�ؼ������ش��ļ���ͼƬ
		UpdateWindow();

		CRect rect1;//���������
		CRect rect2;
		CRect rect3;

		m_axial.GetClientRect(&rect1); //���pictrue�ؼ����ڵľ�������
		m_coronal.GetClientRect(&rect2);
		m_sagittal.GetClientRect(&rect3);

		CDC *pDc1 = m_axial.GetDC();   //���pictrue�ؼ���Dc
		CDC *pDc2 = m_coronal.GetDC();
		CDC *pDc3 = m_sagittal.GetDC();

		SetStretchBltMode(pDc1->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc2->m_hDC, STRETCH_HALFTONE);
		SetStretchBltMode(pDc3->m_hDC, STRETCH_HALFTONE);

		image.StretchBlt(pDc1->m_hDC, rect1, SRCCOPY);
		image_cor.StretchBlt(pDc2->m_hDC, rect2, SRCCOPY);
		image_sag.StretchBlt(pDc3->m_hDC, rect3, SRCCOPY);

		ReleaseDC(pDc1);               //�ͷ�picture�ؼ���Dc
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ͷ�ͼ�񻺴��ļ���
	if (image != NULL)
	  DcmFileProcess::deleteCache();

	CDialogEx::OnOK();
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

void CDcmViewerDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		CRect rect;//���������
		m_coronal.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_coronal.GetDC();   //���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image_cor.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	}

	*pResult = 0;
}

void CDcmViewerDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�༭����slider����
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
		CRect rect;//���������
		m_sagittal.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_sagittal.GetDC();   //���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		image_sag.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc

	}

	*pResult = 0;
}


void CDcmViewerDlg::OnOpenMark()
{
	// TODO:  �ڴ���������������
	CString FileName;
	CFileDialog dialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("All Files(*.*)|*.*||"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		FileName = dialog.GetPathName();
		//MessageBox(FileName);
		DcmFileProcess::showPosPosition(CStringToString(FileName).c_str());
	}
	else
	{
		return;
	}
}
