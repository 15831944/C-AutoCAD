#pragma once
class CConvertUtil
{
public:
	CConvertUtil();
	~CConvertUtil();
	static AcGePoint3d CConvertUtil::ToPoint3d(AcGePoint2d pt);

	// ת��Ϊ�ַ���
	static CString ToString(int val);
	static CString ToString(double value, int precision = 2);

	// �ַ���ת��Ϊ�ض�������
	static double ToDouble(const TCHAR* str);
};

