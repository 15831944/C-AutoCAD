#pragma once
#include"resource.h"

// MFCDialog �Ի���

class MFCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MFCDialog)

public:
	MFCDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MFCDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_MODAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
