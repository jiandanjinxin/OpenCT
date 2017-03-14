
// DcmViewerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDcmViewerDlg �Ի���
class CDcmViewerDlg : public CDialogEx
{
// ����
public:
	CDcmViewerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	//~CDcmViewerDlg();
// �Ի�������
	enum { IDD = IDD_DCMVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	CImage image;        //����ͼƬ��
	CString filepath;    //���ļ����ļ���Ŀ¼

	CStatic m_axial;
	CSliderCtrl m_slider1;
	CEdit m_edit1;
	afx_msg void OnBnClickedOk();
};
