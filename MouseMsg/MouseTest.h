#pragma once
class MouseTest
{
public:
	MouseTest();
	~MouseTest();
	//��������������
	void regCaps();   //ע������
	void unregCaps();  //ע������
//public:
	//static BOOL CapsDone;   //�ж��Ƿ�ע���˸�����
};
BOOL toCaps(MSG* pMsg);
