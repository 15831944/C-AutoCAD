#pragma once


// CTjAreaDlg �Ի���

class CTjAreaDlg : public CDialog
{
	DECLARE_DYNAMIC(CTjAreaDlg)

public:
	CTjAreaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTjAreaDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
};
