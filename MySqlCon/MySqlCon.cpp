// MySqlCon.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MySqlCon.h"
#include"ADOConn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

CString Convert(_variant_t var) {
	CString str;

	if (var.vt == VT_I4)
	{
		long lNum;
		char szCh[21];
		str = var.bstrVal;
		WideCharToMultiByte
			(CP_ACP, 0, var.bstrVal, -1,
				szCh, sizeof(szCh), NULL, NULL);
	}

	switch (var.vt)
	{
	case VT_BOOL:
		str = var.boolVal == 0 ? "0" : "1";
		break;
	case VT_BSTR:         //var is BSTR type
		str = var.bstrVal;
		break;

	case VT_I2:           //var is short int type 
		str.Format(L"%d", (int)var.iVal);
		break;

	case VT_I4:          //var is long int type
		str.Format(L"%d", var.lVal);
		break;

	case VT_R4:         //var is float type
		str.Format(L"%10.6f", (double)var.fltVal);
		break;

	case VT_R8:         //var is double type
		str.Format(L"%10.6f", var.dblVal);
		break;

	case VT_CY:        //var is CY type
		str = COleCurrency(var).Format();
		break;

	case VT_DATE:     //var is DATE type
		str = COleDateTime(var).Format();
		break;

	default:
		str.Format(L"Unk type %d\n", var.vt);
		TRACE(L"Unknown type %d\n", var.vt);
	}
	return str;
}

void MySql() {
	ADOConn m_adoConn;
	m_adoConn.OnInitADOConn();
	CString sql("select * from Tbl_UserKq");
	_RecordsetPtr m_pRecordset;
	m_pRecordset = m_adoConn.GetRecordSet((_bstr_t)sql);




	while (m_pRecordset->adoEOF == 0)
	{
		//strcpy_s(node.nodeName, (char*)(_bstr_t)m_pRecordset->GetCollect("nodeName"));
		//node.nodeLeft = m_pRecordset->GetCollect("nodeLeft");
		//node.nodeRight = m_pRecordset->GetCollect("nodeRight");

		_variant_t name = m_pRecordset->GetCollect("UserName");

		CString uName = Convert(name);

		AfxMessageBox(uName);

		m_pRecordset->MoveNext();
	}
	//�Ͽ����ݿ�
	m_adoConn.ExitConnect();

}

int main()
{
    int nRetCode = 0;

   /* HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: ���Ĵ�������Է���������Ҫ
            wprintf(L"����: MFC ��ʼ��ʧ��\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
        }
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        wprintf(L"����: GetModuleHandle ʧ��\n");
        nRetCode = 1;
    }*/

	MySql();

    return nRetCode;
}



