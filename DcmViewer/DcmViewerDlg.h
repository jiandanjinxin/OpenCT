
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
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);

	CImage image;        //����ͼƬ��
	CImage image_cor;
	CImage image_sig;
	CString filepath;    //���ļ����ļ���Ŀ¼

	CStatic m_axial;
	CStatic m_coronal;
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider3;
	CEdit m_edit1;
	CEdit m_edit3;
	afx_msg void OnBnClickedOk();
};
