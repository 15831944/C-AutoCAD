#pragma once
class CConvertUtil
{
public:
	CConvertUtil();
	~CConvertUtil();
	static AcGePoint3d CConvertUtil::ToPoint3d(AcGePoint2d pt);
	static AcGePoint2d CConvertUtil::ToPoint2d(AcGePoint3d pt);

	// ת��Ϊ�ַ���
	static CString ToString(int val);
	static CString ToString(double value, int precision = 2);

	// �ַ���ת��Ϊ�ض�������
	static double ToDouble(const TCHAR* str);

	//��������ת�û�����
	static AcGePoint3d WcsToUcsPoint(const AcGePoint3d &point);

	//�û�����ת��������
	static AcGePoint3d UcsToWcsPoint(const AcGePoint3d &point);

};

