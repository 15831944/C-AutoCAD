#include "stdafx.h"
#include "DatabaseJigEntity.h"
CDatabaseJigEntity::CDatabaseJigEntity()
{
}


CDatabaseJigEntity::~CDatabaseJigEntity()
{
}

Adesk::Boolean CDatabaseJigEntity::subWorldDraw(AcGiWorldDraw * mode)
{
	//����ط��ǹؼ���
	mode->geometry().pushModelTransform(m_Xform);
	AcDbEntity* pEnt;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		//����ʵ������д��
		if (Acad::eOk == acdbOpenObject(pEnt, m_Ids[i], AcDb::kForRead))
		{
			mode->geometry().draw(pEnt);
			pEnt->close();
		}
	}
	//��䲻���٣��ָ��ֳ�
	mode->geometry().popModelTransform();
	return (AcDbEntity::subWorldDraw(mode));
}
//�����϶������󣬽�ʵ��任����λ��
BOOL CDatabaseJigEntity::transform()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt=NULL;
	AcDbObject *pObj=NULL;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		pEnt->transformBy(m_Xform);
	}
	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}
bool CDatabaseJigEntity::move() {

	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt=NULL;

	AcDbObject *pObj=NULL;	
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		
		pEnt->transformBy(m_Xform);
		pEnt->close();
	}
	return true;
}

//�����϶������󣬽�ʵ�帴�Ƶ���λ��
BOOL CDatabaseJigEntity::transformedCopy()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt=NULL;
	AcDbEntity *pNewEnt=NULL;
	AcDbObject *pObj=NULL;
	AcDbBlockTableRecord *pBlkRec;
	AcDbObjectId blkRecId;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForRead))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		if (0 == i)
		{
			blkRecId = pEnt->blockId();
			if (Acad::eOk != pTrans->getObject(pObj, blkRecId, AcDb::kForWrite))
			{
				acTransactionManagerPtr()->abortTransaction();
				return FALSE;
			}
			pBlkRec = AcDbBlockTableRecord::cast(pObj);
		}
		pEnt->getTransformedCopy(m_Xform, pNewEnt);
		pBlkRec->appendAcDbEntity(pNewEnt);
		acTransactionManagerPtr()->addNewlyCreatedDBRObject(pNewEnt);
	}
	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}
