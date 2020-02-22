//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"SelectUtil.h"
#include"GetInputUtil.h"
#include"MathUtil.h"
#include"DwgDataBaseUtil.h"
#include"GePointUtil.h"
#include<vector>
using namespace std;

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CDongTaiGongXianApp : public AcRxArxApp {

public:
	CDongTaiGongXianApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void ECDMyGroupECDGongX() {
		// Put your command code here

		//��ÿ�
		AcDbEntity * pEnt;
		AcRxClass *cls = AcDbEntity::desc();
		AcGePoint3d ptSel;
		if (!CSelectUtil::PromptSelectEntity(L"��ѡ��Ҫ�ƶ��Ŀ飺\n", cls, pEnt, ptSel, true)) {
			acutPrintf(L"ȡ��ѡ��");
			return;
		}
		AcDbObjectId pEntId = pEnt->objectId();
		AcGePoint3d ptCanZ;

		if (!CGetInputUtil::GetPoint(L"ѡ��Ҫ���ߵĲ��յ㣺\n", ptCanZ)) {

			acutPrintf(L"û��ѡȡ���յ㡣\n");
			pEnt->close();
			return;

		}

		AcGePoint3d minDisPoint;
		AcGePoint3d ptTemp1 = ptCanZ + AcGeVector3d::kXAxis * 5;
		AcGePoint3d ptTemp2 = ptCanZ + AcGeVector3d::kYAxis * 5;

		AcDbLine *l1 = new AcDbLine(ptCanZ, ptTemp1);
		AcDbLine *l2 = new AcDbLine(ptCanZ, ptTemp2);

		if (!getMinDis(pEnt, l1, l2, minDisPoint, ptCanZ)) {

			acutPrintf(L"û�ҵ������\n");
			pEnt->close();
			delete l1;
			l1 = NULL;
			delete l2;
			l2 = NULL;
			return;
		}

		AcDbCircle *circle = new AcDbCircle(minDisPoint, AcGeVector3d::kZAxis, 1);

		AcGePoint3dArray ptJiaoDian1;

		AcDbEntity * MinDisEnt = NULL;
		AcGePoint3d ptCneter= minDisPoint;
		if (pEnt->isKindOf(AcDbBlockReference::desc())) {

			AcDbEntity* copyEnt = (AcDbEntity*)pEnt->clone();

			AcDbVoidPtrArray ptrArr;

			copyEnt->explode(ptrArr);

			vector<double> vecMin;
			vector<AcGePoint3d> vecMinPt;
			//�ҵ����յ����ֽ���ʵ�������Ľ���ľ���
			for (int i = 0; i < ptrArr.length(); i++)
			{
				AcDbEntity * tempEnt = (AcDbEntity*)ptrArr[i];

				AcGePoint3d tempPt;

				if (!getMinDis(tempEnt, l1, l2, tempPt, ptCanZ)) {

					acutPrintf(L"û�ҵ������1\n");
					continue;
				}
				vecMinPt.push_back(tempPt);
				double dis = ptCanZ.distanceTo(tempPt);

				vecMin.push_back(dis);
			}
			if (vecMin.size() == 0) {
				pEnt->close();
				delete l1;
				l1 = NULL;
				delete l2;
				l2 = NULL;
				acutPrintf(L"����û�����");
				return;

			}
			//�ҵ�ÿ��ʵ������յ�������Ǹ��±�
			double min = vecMin[0];
			int entIndex = 0;
			for (int i = 1; i < (int)vecMin.size(); i++)
			{

				if (min > vecMin[i]) {
					entIndex = i;
					min = vecMin[i];

				}

			}


			/*delete copyEnt;
			copyEnt = NULL;*/

			for (int i = 0; i < ptrArr.length() && i != entIndex; i++)
			{
				delete ptrArr[i];
				ptrArr[i] = NULL;
			}
			MinDisEnt = (AcDbEntity*)ptrArr[entIndex];

			MinDisEnt->setColorIndex(1);

		 ptCneter = vecMinPt[entIndex];

			AcDbCircle *circle2 = new AcDbCircle(ptCneter, AcGeVector3d::kZAxis, 1);
			
			AcDbObjectId tempId=CDwgDataBaseUtil::PostToModelSpace(MinDisEnt);

			MinDisEnt->close();

			acdbOpenObject(MinDisEnt, tempId, AcDb::kForWrite);



			circle2->intersectWith(MinDisEnt, AcDb::kOnBothOperands, ptJiaoDian1, 0, 0);

			MinDisEnt->close();
			CDwgDataBaseUtil::PostToModelSpace(circle2);
			circle2->close();
		}

		if (l1 != NULL)
		{
			CDwgDataBaseUtil::PostToModelSpace(l1);
			l1->close();
		}
		if (l2 != NULL)
		{
			CDwgDataBaseUtil::PostToModelSpace(l2);
			l2->close();
		}

		if (!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			circle->intersectWith(pEnt, AcDb::kOnBothOperands, ptJiaoDian1, 0, 0);
		}
		CDwgDataBaseUtil::PostToModelSpace(circle);
		circle->close();
		
		if (MinDisEnt != NULL) {
			MinDisEnt->close();
		}

		if (ptJiaoDian1.length() >= 2) {

			AcGeVector3d vec1;
			AcGeVector3d vec2;

			if (!getLine(ptCneter, ptJiaoDian1, vec1, vec2)) {

				/*delete circle;
				circle = NULL;*/
				pEnt->close();

				
				acutPrintf(L"����������\n");
			}
			else {

				AcGePoint3d pt0 = ptCneter + vec1 * 10;
				AcGePoint3d pt1 = ptCneter + vec2 * 10;

				AcDbLine * line = new AcDbLine(pt0, pt1);

				line->setColorIndex(1);
				CDwgDataBaseUtil::PostToModelSpace(line);

				line->close();
				/*delete circle;
				circle = NULL;*/
				/*CDwgDataBaseUtil::PostToModelSpace(circle);
				circle->close();*/
				AcGeVector3d getVec1 = pt1 - pt0;

				AcGePoint3d ptKaoJin;
				AcDbEntity * pEntKaoJin;
				
				
				
				while (CSelectUtil::PromptSelectEntity(L"��ѡ��Ҫ�����Ķ������ǿ飺\n", cls,
					pEntKaoJin, ptKaoJin, true)) {
					
					AcGePoint3d ptTemp1 = ptKaoJin + AcGeVector3d::kXAxis * 5;
					AcGePoint3d ptTemp2 = ptKaoJin + AcGeVector3d::kYAxis * 5;

					AcDbLine *l1 = new AcDbLine(ptKaoJin, ptTemp1);
					AcDbLine *l2 = new AcDbLine(ptKaoJin, ptTemp2);
					AcGePoint3d disPoint2;
					
					if (!getMinDis(pEntKaoJin, l1, l2, disPoint2, ptKaoJin)) {

						acutPrintf(L"û�ҵ������3\n");
						pEntKaoJin->close();
						delete l1;
						l1 = NULL;
						delete l2;
						l2 = NULL;
						return;
					}


					AcDbCircle *circle3 = new AcDbCircle(disPoint2, AcGeVector3d::kZAxis, 1);


					AcGePoint3dArray ptJiaoDian3;

					if (pEntKaoJin->isKindOf(AcDbBlockReference::desc()))
					{
						acutPrintf(L"�����ǿ�\n");
						pEntKaoJin->close();
						return;
						
					}
					circle3->intersectWith(pEntKaoJin, AcDb::kOnBothOperands, ptJiaoDian3, 0, 0);
					
					CDwgDataBaseUtil::PostToModelSpace(circle3);
					circle3->close();

					AcGeVector3d moveVec = disPoint2 - ptCneter;

					pEnt->transformBy(AcGeMatrix3d::translation(moveVec));


					AcGeVector3d getVec2;

					if (ptJiaoDian3.length() >= 2) {

						AcGeVector3d vec11;
						AcGeVector3d vec22;

						if (!getLine(disPoint2, ptJiaoDian3, vec11, vec22)) {

							/*delete circle;
							circle = NULL;*/
							pEntKaoJin->close();


							acutPrintf(L"����������2\n");
						}
						else {

							AcGePoint3d pt00 = disPoint2 + vec11 * 10;
							AcGePoint3d pt11 = disPoint2 + vec22 * 10;
							getVec2 = pt11 - pt00;
							AcDbLine * line1 = new AcDbLine(pt00, pt11);

							line1->setColorIndex(1);
							CDwgDataBaseUtil::PostToModelSpace(line1);

							line1->close();

							double angle = getVec2.angleTo(getVec1);

							getVec2.transformBy(AcGeMatrix3d::rotation(angle, AcGeVector3d::kZAxis, disPoint2));

							double tempAngle = getVec2.angleTo(getVec1);

							double temp = tempAngle / CMathUtil::PI()*180.0;

							if (fabs(temp - 180) > 2) {

								angle *= -1;

							}

							pEnt->transformBy(AcGeMatrix3d::rotation(angle, AcGeVector3d::kZAxis, disPoint2));
							getVec1.transformBy(AcGeMatrix3d::rotation(angle, AcGeVector3d::kZAxis, disPoint2));
							ptCneter = disPoint2;
						}

						pEnt->close();

						acdbOpenObject(pEnt, pEntId, AcDb::kForWrite);

						pEntKaoJin->close();
					}
					else {

						pEntKaoJin->close();
						/*delete circle;
						circle = NULL;*/
						acutPrintf(L"û����2\n");
					}

				}

				pEnt->close();

			}
		}
		else {
			pEnt->close();
			/*delete circle;
			circle = NULL;*/
			acutPrintf(L"û����\n");
		}
	}
	static bool  getLine(AcGePoint3d pt1, AcGePoint3dArray ptArr, AcGeVector3d &vec1, AcGeVector3d& vec2) {

		vector<AcGeVector3d> vecs;

		for (int i = 0; i < ptArr.length(); i++)
		{
			AcGeVector3d vec = ptArr[i] - pt1;

			vecs.push_back(vec);

		}

		for (int i = 0; i < (int)vecs.size(); i++)
		{

			vec1 = vecs[i];

			for (int j = i + 1; j < (int)vecs.size(); j++)
			{

				vec2 = vecs[j];
				double jj = vec1.angleTo(vec2);

				double temp = jj / CMathUtil::PI()*180.0;
				CString str;
				str.Format(L"%f,%f\n", jj, temp);

				acutPrintf(str);

				if (temp >= 178 || temp <= 2) {

					return true;
				}
			}
		}
		return false;
	}

	static bool getMinDis(AcDbEntity* pEnt, AcDbLine *l1, AcDbLine *l2, AcGePoint3d &minDisPoint, AcGePoint3d ptCanZ) {

		AcGePoint3dArray ptArr1;

		l1->intersectWith(pEnt, Intersect::kExtendThis, ptArr1, 0, 0);

		if (ptArr1.length() == 0) {

			l2->intersectWith(pEnt, Intersect::kExtendThis, ptArr1, 0, 0);

		}
		if (ptArr1.length() == 0) {
			acutPrintf(L"��ʼû�н���\n");

			return false;
		}
		int smallIndex = 0;
		double dis = ptCanZ.distanceTo(ptArr1[0]);
		for (int i = 1; i < ptArr1.length(); i++)
		{
			double dis2 = ptCanZ.distanceTo(ptArr1[i]);

			if (dis2 < dis) {

				dis = dis2;
				smallIndex = i;
			}

		}
		minDisPoint = ptArr1[smallIndex];

		return true;

	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CDongTaiGongXianApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CDongTaiGongXianApp, ECDMyGroup, ECDGongX, ECDGongX, ACRX_CMD_MODAL, NULL)

