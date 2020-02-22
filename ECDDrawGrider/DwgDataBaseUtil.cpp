#include "stdafx.h"
#include "DwgDataBaseUtil.h"


CDwgDataBaseUtil::CDwgDataBaseUtil()
{
}


CDwgDataBaseUtil::~CDwgDataBaseUtil()
{
}

AcDbObjectId CDwgDataBaseUtil::PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb) {

	if (pEnt == NULL) {
		return AcDbObjectId::kNull;
	}
	AcDbBlockTable *pTable;

	AcDbBlockTableRecord *blkTblRec;

	AcDbObjectId objId;

	pDb->getBlockTable(pTable, AcDb::OpenMode::kForRead);
	pTable->getAt(ACDB_MODEL_SPACE, blkTblRec,AcDb::OpenMode::kForWrite);
	
	pTable->close();
	
	Acad::ErrorStatus status=blkTblRec->appendAcDbEntity(objId,pEnt);

	if (status != Acad::eOk) {

		blkTblRec->close();
		
		delete pEnt;
		pEnt = NULL;

		return AcDbObjectId::kNull;

	}

	blkTblRec->close();
	pEnt->close();

	return objId;
}

AcDbObjectIdArray CDwgDataBaseUtil::GetAllEntityIds(const TCHAR* layerName, AcDbDatabase *pDb)
{
	AcDbObjectIdArray entIds;		// ����������ʵ�弯��
	bool bFilterLayer = false;			// �Ƿ���Ҫ����ͼ��
	AcDbObjectId layerId;
	// ���ָ��ͼ��Ķ���ID
	if (layerName != NULL)
	{
		AcDbLayerTable *pLayerTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTbl, AcDb::kForRead);
		if (!pLayerTbl->has(layerName))
		{
			pLayerTbl->close();
			return entIds;
		}
		pLayerTbl->getAt(layerName, layerId);
		pLayerTbl->close();

		bFilterLayer = true;
	}

	// ��ÿ��
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);

	// ���ģ�Ϳռ�Ŀ���¼
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	// ���������������η���ģ�Ϳռ��ÿһ��ʵ��
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	for (it->start(); !it->done(); it->step())
	{
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es = it->getEntity(pEnt, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (bFilterLayer)				// ����ͼ��
			{
				if (pEnt->layerId() == layerId)
				{
					entIds.append(pEnt->objectId());
				}
			}
			else
			{
				entIds.append(pEnt->objectId());
			}

			pEnt->close();
		}
		else
		{
			acutPrintf(TEXT("\nCDwgDatabaseUtil::GetAllEntityIds�д�ʵ��ʧ��(�������:%d)."), (int)es);
		}
	}
	delete it;
	pBlkTblRcd->close();

	return entIds;
}