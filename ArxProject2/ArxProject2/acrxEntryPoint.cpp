//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"RunCommand.h"
#include"EllipseUtil.h"
#include"SplieUtil.h"
#include"PolylineUtil.h"
#include"MathUtil.h"
#include"RegionUtil.h"
#include"TextUtil.h"
#include"HatchUtil.h"
#include"Dimension.h"
#include"GePointUtil.h"
#include"LineUtil.h"
#include"CircleUtil.h"
#include"TransUtil.h"
#include"BlockUtil.h"
#include"LayerUtil.h"
#include"TextFileUtil.h"
#include"StringUtil.h"
#include"ConvertUtil.h"
#include"TextStyleUtil.h"
#include"acedCmdNF.h"
#include"GetInputUtil.h"
#include"SelectUtil.h"
#include"DrawRecJig.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject1App : public AcRxArxApp {

public:
	CArxProject1App() : AcRxArxApp() {}

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

	// ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupMyCommand() {
		// Put your command code here
		AfxMessageBox(TEXT("Hello World!"));
	}
	//����ֱ��
	static void ECDMyGroupMyLine() {

		RunCommand run = RunCommand();

		AcDbObjectId lineId = run.CreateLine();

		run.ChangeColor(lineId, 1);

	}

	static void ECDMyGroupAddEllipse() {

		AcGeVector3d vecNormal(0, 0, 1);

		AcGeVector3d majorAxis(40, 0, 0);

		AcDbObjectId entId;

		entId = CEllipseUtil::Add(AcGePoint3d::kOrigin, vecNormal, majorAxis, 0.5);

		AcGePoint2d pt1(60, 80), pt2(140, 120);

		CEllipseUtil::Add(pt1, pt2);

	}

	static void ECDMyGroupAddSpline() {

		AcGePoint3d pt1(0, 0, 0), pt2(10, 30, 0), pt3(60, 80, 0), pt4(100, 100, 0);

		AcGePoint3dArray points;

		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		points.append(pt4);

		CSplieUtil::Add(points, 4, 0.0);

		pt2.set(30, 10, 0);
		pt3.set(80, 60, 0);
		points.removeSubArray(0, 3);

		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		points.append(pt4);

		AcGeVector3d startTangent(5, 1, 0);
		AcGeVector3d endTangent(5, 1, 0);

		CSplieUtil::Add(points, startTangent, endTangent, 4, 0.0);

	}

	static void ECDMyGroupAddPolyline() {

		AcGePoint2d &ptCenter = AcGePoint2d(200, 200);

		AcGePoint2d &ptCenter2 = AcGePoint2d(300, 300);

		double rotation = 45.0 / 180 * CMathUtil::PI();

		CPolylineUtil::AddPolygon(ptCenter, 6, 30, rotation, 1);

		CPolylineUtil::AddPolyCircle(ptCenter2, 40, 1);
	}

	static void ECDMyGroupAddRegion() {

		ads_name ss;

		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);

		AcDbObjectIdArray objIds;

		if (rt == RTNORM) {

			int length;

			acedSSLength(ss, &length);

			for (int i = 0; i < length; i++)
			{
				ads_name ent;

				acedSSName(ss, i, ent);

				AcDbObjectId objId;

				acdbGetObjectId(objId, ent);

				objIds.append(objId);
			}
		}
		acedSSFree(ss);

		AcDbObjectIdArray regionIds;

		regionIds = CRegionUtil::Add(objIds);

		int number = regionIds.length();

		acutPrintf(TEXT("\n�Ѿ�������%d������"), number);

	}

	static void ECDMyGroupAddText() {

		AcGePoint3d ptInsert(0, 4, 0);

		CTextUtil::AddText(ptInsert, TEXT("LL_ABC"));

		ptInsert.set(0, 0, 0);

		CTextUtil::AddMText(ptInsert, TEXT("��������\n123\n"));

	}

	static void ECDMyGroupAddHatch() {

		ads_name ss;

		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);

		AcDbObjectIdArray objIds;

		if (rt == RTNORM) {

			int length;
			acedSSLength(ss, &length);

			for (int i = 0; i < length; i++)
			{

				ads_name ent;

				acedSSName(ss, i, ent);

				AcDbObjectId objId;

				acdbGetObjectId(objId, ent);

				objIds.append(objId);

			}

		}
		acedSSFree(ss);

		CHatchUtil::Add(objIds, TEXT("BOX"), 0.1);
	}

	static void ECDMyGroupAddDimension() {
		// ָ����ʼ��λ��
		AcGePoint3d pt1(200, 160, 0);
		AcGePoint3d pt2 = CGePointUtil::RelativePoint(pt1, -40, 0);
		AcGePoint3d pt3 = CGePointUtil::PolarPoint(pt2,
			7 * CMathUtil::PI() / 6, 20);
		AcGePoint3d pt4 = CGePointUtil::RelativePoint(pt3, 6, -10);
		AcGePoint3d pt5 = CGePointUtil::RelativePoint(pt1, 0, -20);

		// ������������
		CLineUtil::Add(pt1, pt2);
		CLineUtil::Add(pt2, pt3);
		CLineUtil::Add(pt3, pt4);
		CLineUtil::Add(pt4, pt5);
		CLineUtil::Add(pt5, pt1);

		// ����Բ��
		AcGePoint3d ptCenter1, ptCenter2;
		ptCenter1 = CGePointUtil::RelativePoint(pt3, 16, 0);
		ptCenter2 = CGePointUtil::RelativePoint(ptCenter1, 25, 0);
		CCircleUtil::Add(ptCenter1, 3);
		CCircleUtil::Add(ptCenter2, 4);

		AcGePoint3d ptTemp1, ptTemp2;
		// ˮƽ��ע
		ptTemp1 = CGePointUtil::RelativePoint(pt1, -20, 3);
		CDimension::AddDimRotated(0, pt1, pt2, ptTemp1, NULL);

		// ��ֱ��ע
		ptTemp1 = CGePointUtil::RelativePoint(pt1, 4, 10);
		CDimension::AddDimRotated(CMathUtil::PI() / 2, pt1, pt5, ptTemp1, NULL);

		// ת�Ǳ�ע
		ptTemp1 = CGePointUtil::RelativePoint(pt3, -3, -6);
		CDimension::AddDimRotated(7 * CMathUtil::PI() / 4, pt3, pt4, ptTemp1, NULL);

		// �����ע
		ptTemp1 = CGePointUtil::RelativePoint(pt2, -3, 4);
		CDimension::AddDimAligned(pt2, pt3, ptTemp1,
			AcGeVector3d(4, 10, 0), TEXT("new position"));

		// �Ƕȱ�ע
		ptTemp1 = CGePointUtil::RelativePoint(pt5, -5, 5);
		CDimension::AddDim3PtAngular(pt5, pt1, pt4, ptTemp1);

		// �뾶��ע
		ptTemp1 = CGePointUtil::PolarPoint(ptCenter1,
			CMathUtil::PI() / 4, 3);
		CDimension::AddDimRadial(ptCenter1, ptTemp1, -3);

		// ֱ����ע
		ptTemp1 = CGePointUtil::PolarPoint(ptCenter2, CMathUtil::PI() / 4, 4);
		ptTemp2 = CGePointUtil::PolarPoint(ptCenter2, CMathUtil::PI() / 4, -4);
		CDimension::AddDimDiametric(ptTemp1, ptTemp2, 0);

		// �����ע
		CDimension::AddDimOrdinate(ptCenter2, AcGeVector3d(0, -10, 0),
			AcGeVector3d(10, 0, 0));

	}

	static void ECDMyGroupMyDrag() {

		ads_name  entName;
		ads_point pt;

		if (acedEntSel(TEXT("��ѡ��Բ��"), entName, pt) != RTNORM) {
			return;
		}
		//��ö����ָ��
		AcDbObjectId arcId = NULL;
		if (acdbGetObjectId(arcId, entName) != ErrorStatus::eOk)
			return;

		//ͨ��id���ʵ�����
		AcDbEntity *pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, arcId, AcDb::OpenMode::kForRead) != ErrorStatus::eOk)
			return;

		//�ж�ʵ������Ƿ���Բ��
		if (!pEnt->isKindOf(AcDbArc::desc())) {

			pEnt->close();
			return;

		}

		//��ʵ��ת��ΪԲ��
		AcDbArc *pArc = AcDbArc::cast(pEnt);

		AcGePoint3d ptCenter = pArc->center();
		AcGePoint3d ptStart, ptEnd, ptMiddle;
		pArc->getStartPoint(ptStart);
		pArc->getEndPoint(ptEnd);

		double length = 0.0;
		pArc->getDistAtPoint(ptEnd, length);
		pArc->getPointAtDist(length / 2, ptMiddle);

		pEnt->close();

		//��������Ƕȱ�ע
		CString strLength;
		strLength.Format(_T("%.2f"), length);

		AcDbObjectId dimId;

		dimId = CDimension::AddDim3PointAngular(ptCenter, ptStart, ptEnd, ptMiddle, strLength, NULL);

		//�϶����ı��ע���ֵ�λ��
		AcGePoint3d ptText;
		int track = 1, type;
		struct resbuf result;

		while (track > 0) {

			acedGrRead(track, &type, &result);

			ptText.x = result.resval.rpoint[X];
			ptText.y = result.resval.rpoint[Y];

			//��������dim�����ֱ�עλ��

			if (acdbOpenAcDbEntity(pEnt, dimId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

				AcDb3PointAngularDimension *dim3;

				if (pEnt->isKindOf(AcDb3PointAngularDimension::desc())) {

					dim3 = AcDb3PointAngularDimension::cast(pEnt);

					dim3->setTextPosition(ptText);
				}

				pEnt->close();
			}

			if (type == 3) {

				track = 0;

			}
		}
	}

	static void ECDMyGroupMyDrag2() {

		ads_name entName;
		ads_point ptBase, ptPick;

		if (acedEntSel(_T("��ѡ��������"), entName, ptPick) != RTNORM)
			return;

		AcDbObjectId textId;
		if (acdbGetObjectId(textId, entName) != Acad::eOk)
			return;

		AcDbEntity *pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, textId, AcDb::OpenMode::kForRead) != Acad::eOk)
			return;

		AcDbText *t;

		if (!pEnt->isKindOf(AcDbText::desc())) {
			pEnt->close();
			return;
		}
		t = AcDbText::cast(pEnt);

		AcGePoint3d ptInsertOld = t->position();

		pEnt->close();

		if (acedGetPoint(NULL, _T("\nѡ�����"), ptBase) != RTNORM)
			return;


		acedPrompt(_T("\n������ڶ�����"));

		AcGePoint3d ptInsertNew(0, 0, 0);
		AcGePoint3d ptPick3d = asPnt3d(ptBase);

		int track = 1;
		int type;
		struct resbuf result;

		while (track > 0) {

			acedGrRead(track, &type, &result);

			if (acdbOpenAcDbEntity(pEnt, textId, AcDb::OpenMode::kForWrite) == Acad::eOk) {

				AcDbText *text = AcDbText::cast(pEnt);

				if (text != NULL) {

					ptInsertNew.x = result.resval.rpoint[X] - (ptPick3d.x - ptInsertOld.x);
					ptInsertNew.y = result.resval.rpoint[Y] - (ptPick3d.y - ptInsertOld.y);

					text->setPosition(ptInsertNew);

				}
				pEnt->close();

			}

			if (type == 3) {
				track = 0;
			}

		}




	}

	static void ECDMyGroupMyTrans() {

		AcDbObjectId oId = CTransUtil::GetId(TEXT("ѡ��"));

		AcDbObjectId newId = CTransUtil::MyClone(oId);

		ads_point basePt;
		//ads_point movePt;

		acedGetPoint(NULL, _T("���������"), basePt);
		//acedGetPoint(NULL, _T("�������յ�"), movePt);

		AcGePoint3d pt1, pt2;
		pt1 = asPnt3d(basePt);
		//pt2 = asPnt3d(movePt);

		//CTransUtil::Move(pt1, pt2, oId);

		//CTransUtil::Rotate(pt1, CMathUtil::PI() / 2, oId);
		CTransUtil::Scale(newId, 3, pt1);

	}

	static void ECDMyGroupMyBlk() {

		AcDbObjectId recId = BlockUtil::CreateBlk();

		//BlockUtil::InsertBlk(recId);

		//wchar_t brName[40];

		//acedGetString(1, L"\n�����������", brName);

		//BlockUtil::InsertBlk(brName);

		BlockUtil::InsertBlkWidthAttr(recId, AcGePoint3d(0, 0, 0));

		AcDbBlockReference *br = NULL;

		AcDbObjectId brId = CTransUtil::GetId(L"ѡ������");
		AcDbEntity *pEnt = NULL;

		if (acdbOpenObject(pEnt, brId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			br = AcDbBlockReference::cast(pEnt);

			if (br != NULL) {


				BlockUtil::SetAttribute2Br(br, _T("�ܳ�"), _T("10"));
				BlockUtil::SetAttribute2Br(br, _T("���"), _T("200"));

				br->close();


			}
			pEnt->close();

		}


	}

	static void ECDMyGroupMyLayer() {
		//CLayerUtil::Add(TEXT("TT"), 1);
		/*AcDbObjectId lId = CLayerUtil::GetLayerId(L"TT");

		int64_t t = lId.asOldId();

		//ACHAR* s = reinterpret_cast<ACHAR*>(t);

		//acutPrintf(s);

		if (CLayerUtil::SetColor(L"TT", 2))
		acutPrintf(L"setColor success\n");
		else
		acutPrintf(L"setColor fail");*/


		/*AcDbObjectIdArray lIds;

		CLayerUtil::GetLayerList(lIds);

		int a = lIds.length();

		//int ת ACHAR
		wchar_t m_reportFileName[256];
		swprintf_s(m_reportFileName, L"%d", a);

		acutPrintf(m_reportFileName);*/

		//ɾ����
		/*wchar_t layerName[100];

		acedGetString(1, L"\n���������:", layerName);

		AcDbObjectId lId = CLayerUtil::GetLayerId(layerName);
		if (!lId.isNull()) {

		AcDbBlockTable *pTable = NULL;

		if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTable, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk) {

		acutPrintf(L"\nδ�ܴ򿪿��");
		return;
		}
		AcDbBlockTableIterator *bIter = NULL;

		pTable->newIterator(bIter);

		for (bIter->start(); !bIter->done(); bIter->step()) {

		AcDbBlockTableRecord *bRec = NULL;

		bIter->getRecord(bRec, AcDb::OpenMode::kForWrite);

		AcDbBlockTableRecordIterator *bRecIter = NULL;

		bRec->newIterator(bRecIter);

		for (bRecIter->start(); !bRecIter->done(); bRecIter->step()) {

		AcDbEntity* pEnt = NULL;
		bRecIter->getEntity(pEnt, AcDb::OpenMode::kForWrite);

		if (pEnt->layerId() == lId) {

		pEnt->erase();

		}

		pEnt->close();
		}

		delete bRecIter;

		bRec->close();



		}

		delete bIter;

		pTable->close();
		acutPrintf(L"\n�����ɹ���");

		}
		else {
		acutPrintf(L"\n������������");
		}
		*/

		//����ͼ��
		/*
		AcDbObjectIdArray lIds2;

		CLayerUtil::GetLayerList(lIds2);

		std::vector<CString>lines;

		for (int i = 0; i < lIds2.length(); i++)
		{
		AcDbLayerTableRecord *lRec = NULL;
		std::vector<CString>layerRecInfos;

		if (acdbOpenObject(lRec, lIds2[i]) == ErrorStatus::eOk) {

		//ͼ������
		TCHAR* layerName;

		lRec->getName(layerName);

		layerRecInfos.push_back(layerName);

		acutDelString(layerName);

		//ͼ����ɫ
		AcCmColor color = lRec->color();

		layerRecInfos.push_back(CConvertUtil::ToString(color.colorIndex()));

		//ͼ������
		AcDbLinetypeTableRecord *lineRec = NULL;

		acdbOpenObject(lineRec, lRec->linetypeObjectId());

		TCHAR*lineName;

		lineRec->getName(lineName);
		layerRecInfos.push_back(lineName);

		acutDelString(lineName);
		lineRec->close();


		//ͼ���߿�
		int lineWight = (int)lRec->lineWeight();

		layerRecInfos.push_back(CConvertUtil::ToString(lineWight));

		CString lineInfos = CStringUtil::Join(layerRecInfos, TEXT(","));

		lines.push_back(lineInfos);

		lRec->close();

		}


		}

		const wchar_t* filea = TEXT("������");

		resbuf result;

		acedGetFileD(filea, L"exportLayer.txt", L"txt", 33, &result);

		const wchar_t* fileName=result.resval.rstring;

		// д���ı��ļ�
		CTextFileUtil::Save(fileName, lines);
		*/

		//����ͼ��

		resbuf *result = NULL;

		acedGetFileNavDialog(L"��Ҫ�����ͼ���ļ�", L"aa.txt", L"txt", L"My File Dialog", 33, &result);

		const wchar_t* fileName = (*result).resval.rstring;
		//CString fileName = TEXT("C:\\Users\\LLLLL_PC\\Desktop\\aa.txt");
		if (_taccess(fileName, 0) != -1)
		{
			std::vector<CString>lines;
			CTextFileUtil::Load(fileName, lines);

			for (int i = 0; i < (int)lines.size(); i++)
			{
				CString l = lines[i];
				std::vector<CString>lInfo;

				CStringUtil::Split(l, TEXT(","), lInfo, false);

				CString layerName = lInfo[0];
				AcDbObjectId lId = CLayerUtil::GetLayerId(layerName);
				if (lId.isNull())
				{
					CLayerUtil::Add(layerName);
					lId = CLayerUtil::GetLayerId(layerName);
				}
				if (lId.isNull()) {
					acutPrintf(TEXT("\nidΪnull."));

				}
				AcDbLayerTableRecord *lRec = NULL;
				if (acdbOpenObject(lRec, lId, AcDb::kForWrite) == Acad::eOk)
				{

					AcCmColor c;
					Adesk::UInt16 colorIndex = _ttoi(lInfo[1]);
					c.setColorIndex(colorIndex);
					lRec->setColor(c);


					// ��������
					AcDbLinetypeTable *pLinetypeTbl = NULL;
					AcDbObjectId linetypeId;
					acdbHostApplicationServices()->workingDatabase()
						->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);
					if (pLinetypeTbl->has(lInfo[2]))
					{
						pLinetypeTbl->getAt(lInfo[2], linetypeId);
						lRec->setLinetypeObjectId(linetypeId);
					}
					pLinetypeTbl->close();

					// �����߿�
					AcDb::LineWeight lineWeight = (AcDb::LineWeight)_ttoi(lInfo[3]);
					lRec->setLineWeight(lineWeight);

					lRec->close();
				}
				else {
					acutPrintf(TEXT("\n�򿪲��ʧ��."));
				}

			}

		}
		else {
			acutPrintf(TEXT("\nδ�ҵ�ָ�����ļ�."));
		}
	}

	static void ECDMyGroupMyTextStyle() {

		//���������ʽ
		CString textStyleName = TEXT("��������");

		AcDbObjectId styleId = CTextStyleUtil::GetAt(textStyleName);

		if (styleId.isNull()) {

			styleId = CTextStyleUtil::Add(textStyleName, TEXT("C:\\Program Files\\Autodesk\\AutoCAD 2018\\Fonts\\txt.shx"),
				TEXT("C:\\Program Files\\Autodesk\\AutoCAD 2018\\Fonts\\romanc.shx"));
		}

		std::vector<CString> vec;

		CTextStyleUtil::GetAll(vec);

		int a = (int)vec.size();

		//int ת ACHAR
		wchar_t m_reportFileName[20];
		swprintf_s(m_reportFileName, L"%d", a);

		acutPrintf(m_reportFileName);


	}

	static void ECDMyGroupMyAcedCmd() {
		//acedCommandS
/* acedCommandS(
	  RTSTR, TEXT("Rectangle"),
	  RTSTR, TEXT("10,10,10"),
	  RTSTR,TEXT("D"),
	  RTSTR, TEXT("25"),
	  RTSTR, TEXT("15"),
	  RTSTR, TEXT("10,10"),
	  RTNONE
	  );*/
	  //acedCmdS
	  /*
	  struct resbuf *rb;

	  int rc = RTNORM;

	  ads_point pt1 = { 10,10,10 };
	  ads_real w = 25;
	  ads_real h = 15;

	  rb = acutBuildList(RTSTR, TEXT("Rectangle"),
		  RTPOINT, pt1,
		  RTSTR, TEXT("D"),
		  RTREAL, w,
		  RTREAL, h,
		  RTSTR, TEXT("10,10"),
		  RTNONE
		  );

	  if (rb != NULL) {
		  rc = acedCmdS(rb);
	  }

	  if (rc != RTNORM){

		  acutPrintf(L"\n��������ʧ����");

	  }

	  acedCommandS(RTSTR, TEXT("Zoom"), RTSTR, TEXT("E"), RTNONE);
	  */

	  //�鿴ʵ��resbuf����Ϣ
	  /*
	  ads_name entName;
	  ads_point pt;
	  if (acedEntSel(L"��ѡ��ʵ��", entName, pt) != RTNORM)
		  return;

	  resbuf *rbEnt;
	  resbuf *rb;

	  rbEnt = acdbEntGet(entName);
	  rb = rbEnt;

	  while (rb != NULL) {

		  switch (rb->restype)
		  {
		  case -1:
			  acutPrintf(L"\nͼԪ����%x", rb->resval.rstring);
			  break;
		  case 0:
			  acutPrintf(L"\nͼԪ����,%s", rb->resval.rstring);
			  break;
		  case 8:
			  acutPrintf(L"\nͼ�㣺%s", rb->resval.rstring);
			  break;
		  case 10:
			  acutPrintf(L"\n��Ҫ�㣺(%.2f,%.2f,%.2f)",
				  rb->resval.rpoint[X],
				  rb->resval.rpoint[Y],
				  rb->resval.rpoint[Z]);
			  break;
		  case 40:
			  acutPrintf(L"\n40:˫���ȸ���ֵ��%.4f", rb->resval.rreal);
			  break;
		  case 210:
			  acutPrintf(L"\nƽ�淨����:(%.2f,%.2f,%.2f)",
				  rb->resval.rpoint[X],
				  rb->resval.rpoint[Y],
				  rb->resval.rpoint[Z]);
			  break;
		  default:
			  break;
		  }
		  rb = rb->rbnext;
	  }
	  if (rbEnt != NULL) {
		  acutRelRb(rbEnt);
	  }
	  */
		
	}

	static void ECDMyGroupMyPL() {

		int index = 2;
		AcGePoint3d ptStart;

		if (!CGetInputUtil::GetPoint(L"\n�������һ���㣺", ptStart)) {
			return;
		}

		AcGePoint3d ptPrevious, ptCurrent;

		ptPrevious = ptStart;

		AcDbObjectId polyId;

		while (CGetInputUtil::GetPoint(ptPrevious, L"\n������һ�¸���:", ptCurrent)) {

			if (index == 2) {

				polyId = CPolylineUtil::Add(CConvertUtil::ToPoint2d(ptPrevious), CConvertUtil::ToPoint2d(ptCurrent));
			}
			else if (index > 2) {

				AcDbPolyline *pl = NULL;

				if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
					return;
				pl->addVertexAt(index -1, CConvertUtil::ToPoint2d(ptCurrent), 0, 0, 0);
				pl->close();

			}
			index++;
			ptPrevious = ptCurrent;
		}
	}
	
	static void ECDMyGroupMyPL2() {

		int plIndex = 2;

		AcGePoint3d basePt;

		if (!CGetInputUtil::GetPoint(L"\n�������һ����:", basePt)) {
			return;
		}
		int rc;
		TCHAR keyword[20];
		AcGePoint3d ptPrevious, ptCurrent;

		ptPrevious = basePt;
		AcDbObjectId pId;

		acedInitGet(NULL, TEXT("C W O"));

		
		
		rc = CGetInputUtil::GetPointReturnCode(ptPrevious, L"\n��������һ�����[��ɫC/�߿�W/���O]:", ptCurrent);
		int colorIndex = -1;
		ads_real width=-1;

		while (rc==RTKWORD||rc==RTNORM) {
			
			switch (rc) {
			case RTKWORD:
				if (acedGetInput(keyword) != RTNORM)
					return;

				if (_tcscmp(keyword, TEXT("C")) == 0) {
					if (acedGetInt(L"\n��������ɫ���0-255", &colorIndex) == RTNORM) {

						colorIndex = colorIndex % 256;
					}
				}
				else if (_tcscmp(keyword, TEXT("W")) == 0) {

					if (acedGetReal(L"\n�������߿�", &width)!= RTNORM) {
						width = 0;
					
					}
				}
				else if (_tcscmp(keyword, TEXT("O")) == 0) {

					return;

				}

				break;
			case RTNORM:
			if (plIndex == 2) {

				pId = CPolylineUtil::Add(CConvertUtil::ToPoint2d(ptPrevious), CConvertUtil::ToPoint2d(ptCurrent));
			}

			else if (plIndex > 2) {

				AcDbPolyline *pl = NULL;

				if (acdbOpenObject(pl, pId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
					return;

					pl->addVertexAt(plIndex - 1, CConvertUtil::ToPoint2d(ptCurrent), 0, 0, 0);
					
					if (colorIndex >= 0) {
						AcCmColor c;

						c.setColorIndex(colorIndex);
						pl->setColor(c);
					}
					if(width>-1)
					pl->setConstantWidth(width);

					pl->close();
					
				}
			
			plIndex++;
			ptPrevious = ptCurrent;	

			break;
			}
			
			acedInitGet(NULL, TEXT("C W O"));
			rc = CGetInputUtil::GetPointReturnCode(ptPrevious, L"\n��������һ�����[��ɫC/�߿�W/���O]:", ptCurrent);
		}


	}

	static void ECDMyGroupMySel() {

		//����������ѡ��
		/*struct resbuf rb;
		TCHAR sbuf[10];
		ads_name ssname;

		rb.restype = 0;//ʵ����
		_tcscpy(sbuf, TEXT("CIRCLE"));
		rb.resval.rstring = sbuf;
		rb.rbnext = NULL;

		acedSSGet(TEXT("X"), NULL, NULL, &rb, ssname);

		acedSSFree(ssname);*/

		//����������ѡ��
		/*struct resbuf *rb;
		ads_name ssname;

		rb = acutBuildList(RTDXF0, TEXT("LINE"),
			8, TEXT("0"),
			RTNONE);

		acedSSGet(TEXT("X"), NULL, NULL, rb, ssname);
		acedSSFree(ssname);*/

		//ads_name sset;

		//ѡ��ͼ�����ݿ������е�ʵ��		
		//acedSSGet(L"A", NULL, NULL, NULL, sset);

		//�û�ѡ��
		//int rc=acedSSGet(NULL, NULL, NULL, NULL, sset);

		//������ھͻ�õ�ǰ��PICKFIRSTѡ��
		//acedSSGet(TEXT("I"), NULL, NULL, NULL, sset);

		//ѡ��ָ���Ķ���ΰ�Χ������ʵ��
		/*ads_point pt1, pt2, pt3, pt4;
		struct resbuf *pointlist;

		pt1[X] = pt1[Y] = -100.0; pt1[Z] =0.0 ;
		pt2[X] = -100; pt2[Y] = 100.0; pt2[Z] = 0.0;
		pt3[X] = 100.0; pt3[Y] = 100.0; pt3[Z] = 0.0;
		pt4[X] = 100.0; pt4[Y] = -100; pt4[Z] = 0.0;

		pointlist = acutBuildList(RTPOINT, pt1, RTPOINT, pt2, RTPOINT, pt3,
			RTPOINT, pt4, 0);
		acedSSGet(L"WP", pointlist, NULL, NULL, sset);*/
			
		/*int32_t length;

			acedSSLength(sset, &length);

			acutPrintf(L"\nʵ������%d", length);

			acedSSFree(sset);*/

		/*AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *type = AcDbCircle::desc();

		if (CSelectUtil::PromptSelectEntity(TEXT("\n��ѡ��Բ��"), type, pEnt, pickPoint)) {

			pEnt->setColorIndex(1);
			pEnt->close();

		}*/

		std::vector<AcRxClass*>descs;
		descs.push_back(AcDbLine::desc());
		descs.push_back(AcDbCircle::desc());

		AcDbObjectIdArray entIds;

		if (CSelectUtil::PromptSelectEnts(L"\n��ѡ��Բ��ֱ��:", NULL, descs, entIds)) {

			for (int i = 0; i < entIds.length(); i++)
			{


				AcDbEntity *pEnt = NULL;
				if (acdbOpenObject(pEnt, entIds[i], AcDb::kForWrite) == Acad::eOk) {

					pEnt->setColorIndex(3);
					pEnt->close();

				}
			}
		}
	}

	static void ECDMyGroupMyJig() {

		DrawRecJig jig;
		ads_point pt;
		acedGetPoint(NULL, L"��ѡ�����㣺", pt);

		AcDbObjectId oId;

		if (jig.DoIt(asPnt3d(pt), oId)) {

			jig.entity()->close();

		}
		

	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject1App)
//��������ĺ�
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyLine, MyLine, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddEllipse, AddEllipse, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddSpline, AddSpline, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddPolyline, AddPolyline, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddRegion, AddRegion, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddText, AddText, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddHatch, AddHatch, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddDimension, AddDimension, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyDrag, MyDrag, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyDrag2, MyDrag2, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyTrans, MyTrans, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyBlk, MyBlk, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyLayer, MyLayer, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyTextStyle, MyTextStyle, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyAcedCmd, MyAcedCmd, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyPL, MyPL, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyPL2, MyPL2, ACRX_CMD_MODAL, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MySel, MySel, ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MySel, MySel, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyJig, MyJig, ACRX_CMD_MODAL, NULL)

