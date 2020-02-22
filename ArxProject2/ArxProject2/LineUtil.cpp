#include "stdafx.h"
#include "LineUtil.h"
#include"DwgDataBaseUtil.h"

CLineUtil::CLineUtil()
{
}


CLineUtil::~CLineUtil()
{
}

 AcDbObjectId CLineUtil::Add(const AcGePoint3d &startPoint, const AcGePoint3d &endPoint, AcDbDatabase *pDb)
{
	// ����ֱ��ʵ��
	AcDbLine *pLine = new AcDbLine(startPoint, endPoint);

	// ��ֱ��ʵ����ӵ�ģ�Ϳռ�
	return CDwgDataBaseUtil::PostToModelSpace(pLine, pDb);
}
