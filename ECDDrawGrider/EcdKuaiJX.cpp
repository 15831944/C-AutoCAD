#include "stdafx.h"
#include "EcdKuaiJX.h"
#include"DwgDataBaseUtil.h"
#include"SelectUtil.h"
#include<vector>
//#include"GePointUtil.h"
#include"GetInputUtil.h"
//#include"MathUtil.h"
#include"ConvertUtil.h"
#include"time.h"
using namespace std;
EcdKuaiJX::EcdKuaiJX()
{
}


EcdKuaiJX::~EcdKuaiJX()
{
}

void EcdKuaiJX::Command()
{
	listVec.clear();
	listOId.removeAll();


	AcDbBlockReference *br = NULL;

	AcRxClass *cls = AcDbBlockReference::desc();

	AcGePoint3d pt;

	if (!CSelectUtil::PromptSelectEntity(L"\n��ѡ��Ҫ����Ŀ飺", cls, (AcDbEntity*&)br, pt, true))
		return;


	//�õ�����
	AcDbObjectId pBlkTblRecId;
	pBlkTblRecId = br->blockTableRecord();
	AcDbBlockTableRecord *pBlkTblRec = NULL;

	ErrorStatus es = acdbOpenObject(pBlkTblRec, pBlkTblRecId, AcDb::kForRead);

	if (es != ErrorStatus::eOk) {
		acutPrintf(L"\n��ȡ���¼����%d", es);
		br->close();
		return;
	}
	ACHAR *name;
	pBlkTblRec->getName(name);

	AcGePoint3d ptPos = br->position();
	AcGePoint3d ptPosMirror;


	int ret = 0;

	CGetInputUtil::GetKeyword(L"\n��ѡ������[X/Y]", L"X Y", L"X", 0, ret);

	AcDbExtents bounds;

	br->bounds(bounds);

	AcGePoint3d ptMax = bounds.maxPoint();
	AcGePoint3d ptMin = bounds.minPoint();

	double maxY = abs(ptMax.y - ptMin.y);
	double maxX = abs(ptMax.x - ptMin.x);




	AcDbVoidPtrArray coll;

	br->explode(coll);

	vector<AcDbEntity *> listEnt;

	for (int i = 0; i < coll.length(); i++)
	{
		listEnt.push_back((AcDbEntity *)coll.at(i));
	}
	AcGeMatrix3d mtx = AcGeMatrix3d();

	//Y
	if (ret == 1) {

		AcGePoint3d ptEnd = ptPos + AcGeVector3d::kYAxis * 100;

		AcGeLine3d lineY = AcGeLine3d(ptPos, ptEnd);

		lineY.transformBy(mtx.setTranslation(AcGeVector3d::kXAxis*maxX));

		ptPosMirror = ptPos.transformBy(AcGeMatrix3d::mirroring(lineY));

		MyMirror(listEnt, lineY, 'Y');
	}
	else {

		AcGePoint3d ptEnd = ptPos + AcGeVector3d::kXAxis * 100;

		AcGeLine3d lineX = AcGeLine3d(ptPos, ptEnd);

		lineX.transformBy(mtx.setTranslation(AcGeVector3d::kYAxis*maxY));

		ptPosMirror = ptPos.transformBy(AcGeMatrix3d::mirroring(lineX));

		MyMirror(listEnt, lineX, 'X');

	}
	//�����¿�
	AcDbObjectId breNewId;

	AcDbBlockTable  * pTable = NULL;

	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTable, AcDb::OpenMode::kForWrite);

	if (es != ErrorStatus::eOk)
	{
		acutPrintf(L"\n�򿪿��ʧ�ܣ�%d", es);
		br->close();
		pBlkTblRec->close();
		return;
	}

	AcDbBlockTableRecord *blkRec = new AcDbBlockTableRecord();

	blkRec->setBlockInsertUnits(pBlkTblRec->blockInsertUnits());

	time_t  m_time;

	time(&m_time);
	//char *timeStff=ctime(&m_time);

	//char *timeStff = "LL";
	//char*  תACHAR
	/*size_t len = strlen(timeStff) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr = (wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, timeStff, _TRUNCATE);
	*/

	wchar_t wchar[11] = { 0 };

	swprintf(wchar, 11, L"%d", m_time);

	wcscat(name, wchar);

	blkRec->setName(name);

	//�Ѿ�����ʵ����ӵ��¼�¼��
	for (int i = 0; i < (int)listVec.size(); i++) {

		AcDbEntity * ent = listVec.at(i);

		if (ent != NULL) {

			blkRec->appendAcDbEntity(ent);
		}
	}
	blkRec->setOrigin(ptPosMirror);
	es = pTable->add(breNewId, blkRec);

	if (es != ErrorStatus::eOk) {

		acutPrintf(L"\n%d", es);

	}
	//��ԭ����ɾ������Ϊ�ھ�������֮ǰ��Ҫ��ը�����ԭ���ּ��뵽ģ�Ϳռ�
	for each (AcDbObjectId  oId in listOId)
	{

		AcDbEntity * ent = NULL;

		if (acdbOpenObject(ent, oId, AcDb::kForWrite) == ErrorStatus::eOk) {


			//ent->erase();

			ent->close();

		}


	}

	listOId.removeAll();

	//�رվ�����ʵ��
	for (int i = 0; i < (int)listVec.size(); i++)
	{

		listVec.at(i)->close();

	}

	//AcDbBlockReference *brOld = new AcDbBlockReference(ptPos, pBlkTblRec->objectId());
	AcDbBlockReference *brNew = new AcDbBlockReference(ptPosMirror, breNewId);

	//�ر��¿��¼
	blkRec->close();
	pTable->close();

	//CDwgDataBaseUtil::PostToModelSpace(brOld);
	CDwgDataBaseUtil::PostToModelSpace(brNew);


	/*AcDbPoint *pt1 = new AcDbPoint(ptPos);
	pt1->setColorIndex(1);
	CDwgDataBaseUtil::PostToModelSpace(pt1);

	AcDbPoint *pt2 = new AcDbPoint(ptPosMirror);
	pt2->setColorIndex(3);
	CDwgDataBaseUtil::PostToModelSpace(pt2);

	pt1->close();
	pt2->close();*/

	br->close();
	pBlkTblRec->close();
}

void EcdKuaiJX::MirrorText(AcDbText *text, AcGeLine3d mirrorLine)
{

	AcDbText *dbText = NULL;

	if (acdbOpenObject(dbText, (*text).objectId(), AcDb::kForWrite) != ErrorStatus::eOk)
		return;

	AcDbText * mirroredTxt = AcDbText::cast(dbText->clone());

	dbText->close();
	text->close();

	AcGeMatrix3d mirrorMatrix = AcGeMatrix3d::mirroring(mirrorLine);


	mirroredTxt->transformBy(mirrorMatrix);

	AcGePoint3d pt1, pt2, pt3, pt4;

	GetTextBoxCorners(*dbText, pt1, pt2, pt3, pt4);

	AcDbPolyline *pl = new AcDbPolyline();

	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt1), 0, 0, 0);
	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt2), 0, 0, 0);
	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt3), 0, 0, 0);
	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt4), 0, 0, 0);

	pl->setColorIndex(1);

	CDwgDataBaseUtil::PostToModelSpace(pl);

	pl->close();

	AcGeVector3d rotDir =
		(pt4 - pt1.asVector()).asVector();

	AcGeVector3d linDir = (pt3 - (pt1.asVector())).asVector();

	AcGeVector3d mirRotDir = rotDir.transformBy(mirrorMatrix);

	AcGeVector3d mirLinDir = linDir.transformBy(mirrorMatrix);

	if (abs(mirrorLine.direction().y) > abs(mirrorLine.direction().x)) {

		
			 mirroredTxt->mirrorInX(!mirroredTxt->isMirroredInX());

		mirroredTxt->setPosition(mirroredTxt->position() + mirLinDir);
	}
	else {
			
			mirroredTxt->mirrorInY(!mirroredTxt->isMirroredInY());

		mirroredTxt->setPosition(mirroredTxt->position() + mirRotDir);
	}
	listVec.push_back(mirroredTxt);


}

void EcdKuaiJX::MirrorText(AcDbMText *mText, AcGeLine3d mirrorLine)
{
}

void EcdKuaiJX::GetTextBoxCorners(AcDbText &dbText, AcGePoint3d &pt1, AcGePoint3d &pt2, AcGePoint3d &pt3, AcGePoint3d &pt4)
{
	ads_name name;

	int result = acdbGetAdsName(name, dbText.objectId());
	resbuf *buf = NULL;
	buf = acdbEntGet(name);


	struct resbuf *rb=NULL;

	//#define  OL_EBADTYPE   93  /* Bad value type */
	 /*#define  OL_ENTSELPICK 7 Entity selection  (failed pick) */
	acedGetVar(L"ERRNO", rb);
	if(rb!=NULL)
	acutPrintf(L"\nrb=%d", rb->resval.rint);



	ads_point point1, point2;

	if (acedTextBox(buf, point1, point2) != RTNORM) {
		
		struct resbuf *rb1=NULL;

		acedGetVar(L"ERRNO", rb1);

		acutPrintf(L"\nrbbox=%d", rb1->resval.rint);
		acutRelRb(rb1);
	}
	if (buf != NULL)
	acutRelRb(buf);
	if (rb != NULL)
	acutRelRb(rb);
	pt1 = asPnt3d(point1);
	pt2 = asPnt3d(point2);

	AcDbText * pEnt = NULL;

	if (acdbOpenObject(pEnt, dbText.objectId(), AcDb::OpenMode::kForWrite) != ErrorStatus::eOk) {
		return;
	}
	pEnt->mirrorInX(Adesk::kFalse);
	pEnt->mirrorInY(Adesk::kFalse);

	AcGeMatrix3d rotMat = AcGeMatrix3d::rotation(pEnt->rotation(), pEnt->normal(), pt1);

	pt1 = pt1.transformBy(rotMat) + pEnt->position().asVector();
	pt2 = pt2.transformBy(rotMat) + pEnt->position().asVector();

	AcGeVector3d rotDir = AcGeVector3d(
	-sin(pEnt->rotation()),
	cos(pEnt->rotation()),
	0
	);
	pEnt->close();
	AcGeVector3d linDir = rotDir.crossProduct(dbText.normal());


	/*AcGeMatrix3d rotMat = AcGeMatrix3d::rotation(dbText.rotation(),dbText.normal(), pt1);

	pt1 = pt1.transformBy(rotMat) + dbText.position().asVector();
	pt2 = pt2.transformBy(rotMat) + dbText.position().asVector();

	AcGeVector3d rotDir = AcGeVector3d(
		-sin(dbText.rotation()),
		cos(dbText.rotation()),
		0
		);

	AcGeVector3d linDir = rotDir.crossProduct(dbText.normal());*/

	double actualWidth =
		abs((pt2.asVector() - pt1.asVector()).dotProduct(linDir));

	pt3 = pt1 + linDir*actualWidth;
	pt4 = pt2 - linDir*actualWidth;

}

void EcdKuaiJX::MyMirror(vector<AcDbEntity*> listEnt, AcGeLine3d l3d, char xY)
{
	for (int i = 0; i < (int)listEnt.size(); i++)
	{

		AcDbEntity * ent = listEnt.at(i);

		AcDbEntity * ent2 = NULL;

		if (ent->isKindOf(AcDbText::desc()) || ent->isKindOf(AcDbMText::desc())) {
			AcDbObjectId textId = CDwgDataBaseUtil::PostToModelSpace(ent);
			listOId.append(textId);

			ent->close();

		}
		else {

			ent->getTransformedCopy(AcGeMatrix3d::mirroring(l3d), ent2);

			if (ent2->isKindOf(AcDbDimension::desc()) == false) {
				listVec.push_back(ent2);
				
			}

		}

		if (ent->isKindOf(AcDbText::desc())) {

			AcDbText * a = (AcDbText *)ent;

			MirrorText(a, l3d);
		}
		else if (ent->isKindOf(AcDbMText::desc())) {

			AcDbMText *a = (AcDbMText *)ent;

			MirrorText(a, l3d);
		}
		else if ((AcDbDimension *)ent != NULL) {




		}
	}
	for (int i = 0; i < (int)listEnt.size(); i++)
	{
		AcDbEntity *ent1 = listEnt.at(i);
		if (ent1->isKindOf(AcDbText::desc()) || ent1->isKindOf(AcDbMText::desc())) {

			continue;

		}
		ent1->close();
	}

}
