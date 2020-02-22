#include "stdafx.h"
#include "HatchUtil.h"
#include"DwgDataBaseUtil.h"


CHatchUtil::CHatchUtil()
{
}


CHatchUtil::~CHatchUtil()
{
}

AcDbObjectId CHatchUtil::Add(const AcDbObjectIdArray & loopIds, const ACHAR * patName, double patternScale)
{
	//1�����յ�������
	AcDbHatch* pHatch = new AcDbHatch();
	// Set hatch plane
	//2�������ƽ��
	AcGeVector3d normal(0.0, 0.0, 1.0);
	pHatch->setNormal(normal);
	pHatch->setElevation(0.0);

	// Set non associative hatch
	//3���ù�����
	pHatch->setAssociative(Adesk::kTrue);

	// Set hatch pattern to SolidFill type
	//4�������ͼ��
	pHatch->setPattern(AcDbHatch::kPreDefined, patName);

	// Set hatch style to kNormal
	//
	pHatch->setHatchStyle(AcDbHatch::kNormal);
	
	//5������߽�
	pHatch->appendLoop(AcDbHatch::kExternal, loopIds);
	
	//6��ʾ������
	Acad::ErrorStatus es = pHatch->evaluateHatch();
	//7��ӵ�ģ�Ϳռ�
	AcDbObjectId hatchId = CDwgDataBaseUtil::PostToModelSpace(pHatch);

	//��������䣬��������߽���а󶨣�ʹ���ܻ�ñ߽�����޸ĵ�֪ͨ

	if (acdbOpenObject(pHatch, hatchId, AcDb::kForRead) == Acad::eOk) {

		AcDbObjectIdArray assocEntIds;

		pHatch->getAssocObjIds(assocEntIds);

		for (int i = 0; i < assocEntIds.length(); i++)
		{
			AcDbEntity *pEnt = NULL;

			if (acdbOpenObject(pEnt, assocEntIds[i], AcDb::kForWrite) == Acad::eOk) {

				pEnt->addPersistentReactor(hatchId);
				pEnt->close();

			}

		}
		pHatch->close();
	}

	return hatchId;

	/*
	// Construct hatch external boundary
	//
	AcGePoint2dArray vertexPts;
	AcGeDoubleArray vertexBulges;
	vertexPts.setPhysicalLength(0).setLogicalLength(5);
	vertexPts[0].set(2.0, 2.0);
	vertexPts[1].set(8.0, 2.0);
	vertexPts[2].set(8.0, 8.0);
	vertexPts[3].set(2.0, 8.0);
	vertexPts[4].set(2.0, 2.0);
	vertexBulges.setPhysicalLength(0).setLogicalLength(5);
	for (int i = 0; i < 5; i++)
		vertexBulges[i] = 0.0;

	// Append an external loop (rectangle) to hatch boundary

	pHatch->appendLoop(AcDbHatch::kExternal, vertexPts, vertexBulges);
	*/
	

}
