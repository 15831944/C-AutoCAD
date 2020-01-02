
#include "StdAfx.h"
#include "ADOConn.h"

ADOConn::ADOConn(void)
{
}


ADOConn::~ADOConn(void)
{
}


void ADOConn::OnInitADOConn(void)
{
	//��ʼ��ole/com�⻷��
	::CoInitialize(NULL);
	try
	{
		//����connection����
		m_pConnection.CreateInstance("ADODB.Connection");
		//���������ַ���
		//_bstr_t strConnect = "Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Initial Catalog=Learnning;Data Source=COMP-02-2433\SQLEXPRESS";
		_bstr_t strConnect = "driver={SQL Server};uid=sa;pwd=940619.lq;Server=.;Database=Learnning";
		m_pConnection->Open(strConnect, "sa", "940619.lq", adModeUnknown);
	}
	//��׽�쳣
	catch (_com_error e)
	{
		//��ʾ������Ϣ
		AfxMessageBox(e.Description());
	}
}


void ADOConn::ExitConnect(void)
{
	//�رռ�¼��������
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	//�ͷŻ���
	::CoUninitialize();
}


_RecordsetPtr ADOConn::GetRecordSet(_bstr_t bstrSql)
{
	try
	{
		//�������ݿ⣬���connection�ǿգ��������������ݿ�
		if (m_pConnection == NULL)
			OnInitADOConn();
		//������¼����
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		//ȡ�ñ��еļ�¼
		m_pRecordset->Open(bstrSql, m_pConnection.GetInterfacePtr(), adOpenDynamic,
			adLockOptimistic, adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//���ؼ�¼��
	return m_pRecordset;
}


bool ADOConn::ExecuteSQL(_bstr_t bstrSql)
{
	_variant_t recordsAffected;
	try
	{
		//���ݿ��Ƿ�������
		if (m_pConnection == NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSql, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return false;
}



