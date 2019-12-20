//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"SelectUtil.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject3App : public AcRxArxApp {

public:
	CArxProject3App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void ViewXData() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n��ѡ��ʵ�壺\n", cls, pEnt, pickPoint)) {

			CString appName = "xDataApp";

			resbuf * rb = pEnt->xData(appName);

			if (rb != NULL) {

				resbuf *tempRs = rb;

				tempRs = tempRs->rbnext;

				acutPrintf(L"�ַ���:%s\n", tempRs->resval.rstring);
				tempRs = tempRs->rbnext;
				acutPrintf(L"���ͣ�%d\n", tempRs->resval.rint);
				tempRs = tempRs->rbnext;
				acutPrintf(L"�����ͣ�%f\n", tempRs->resval.rreal);
				tempRs = tempRs->rbnext;
				acutPrintf(L"�������ݣ�(%.2f,%.2f,%.2f)\n", tempRs->resval.rpoint[X],
					tempRs->resval.rpoint[Y],
					tempRs->resval.rpoint[Z]);

				acutRelRb(rb);
				
			}
			pEnt->close();
		}
	}

	static void DeleteXData() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n��ѡ��Ҫɾ����չ���ݵ�ʵ�壺\n", cls, pEnt, pickPoint)) {

			CString appName = "xDataApp";

			resbuf * rb = acutBuildList(AcDb::kDxfRegAppName, appName, RTNONE);

			pEnt->setXData(rb);

			acutRelRb(rb);

			pEnt->close();

		}
	}

	static void AddXDicRecord() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n��ѡ��ʵ�壺\n", cls, pEnt, pickPoint)) {

			CString appName = "xRecord";

			pEnt->createExtensionDictionary();

			AcDbObjectId dicId = pEnt->extensionDictionary();

			AcDbDictionary *myDic = NULL;

			if (acdbOpenAcDbObject((AcDbObject*&)myDic, dicId, AcDb::kForWrite) == ErrorStatus::eOk) {

				AcDbXrecord *record = new AcDbXrecord();

				AcDbObjectId xRecId;

				myDic->setAt(appName, record, xRecId);

				resbuf * rb= acutBuildList(
					AcDb::kDxfText, L"�ַ�����������",
					AcDb::kDxfInt16, 2,
					AcDb::kDxfReal, 3.14,
					AcDb::kDxfXCoord, asDblArray(pickPoint),
					RTNONE);
				
				record->setFromRbChain(*rb);

				acutRelRb(rb);
				record->close();
				myDic->close();
				pEnt->close();


				acutPrintf(L"\n�����չ��¼�ɹ�");
			}




		}


	}

	static void ViewXDicRecord() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n��ѡ��ʵ�壺\n", cls, pEnt, pickPoint)) {

			CString appName = "xRecord";
		
			AcDbObjectId dicId = pEnt->extensionDictionary();
			pEnt->close();
			if (dicId.isNull()==false) {

				AcDbDictionary *dic = NULL;

				if (acdbOpenObject(dic, dicId, AcDb::kForRead) == ErrorStatus::eOk) {


					AcDbXrecord * record = NULL;

					dic->getAt(appName, record);

						if (record!=NULL) {

							resbuf * rb = NULL;

							record->rbChain(&rb);

							record->close();
							dic->close();

							if (rb != NULL) {

								resbuf *tempRs = rb;

								acutPrintf(L"�ַ���:%s\n", tempRs->resval.rstring);
								tempRs = tempRs->rbnext;
								acutPrintf(L"���ͣ�%d\n", tempRs->resval.rint);
								tempRs = tempRs->rbnext;
								acutPrintf(L"�����ͣ�%f\n", tempRs->resval.rreal);
								tempRs = tempRs->rbnext;
								acutPrintf(L"�������ݣ�(%.2f,%.2f,%.2f)\n", tempRs->resval.rpoint[X],
									tempRs->resval.rpoint[Y],
									tempRs->resval.rpoint[Z]);

								acutRelRb(rb);

							}
							
						}
					}
					else {

						acutPrintf(L"\n���ֵ���������Ƶ���չ��¼��\n");

					}
				}			
			else {
				acutPrintf(L"\n��ʵ������չ���ݡ�\n");

			}

			pEnt->close();
		}


	}

	static void DeleteXDicRecord() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n��ѡ��ʵ�壺\n", cls, pEnt, pickPoint)) {

			CString appName = "xRecord";

			AcDbObjectId dicId = pEnt->extensionDictionary();
			pEnt->close();
			if (dicId.isNull() == false) {
				AcDbDictionary *dic = NULL;
				if (acdbOpenObject(dic, dicId, AcDb::kForWrite) == ErrorStatus::eOk) {

				ErrorStatus es=	dic->remove(appName);

				if (es == ErrorStatus::eOk) {

					acutPrintf(L"\ndelete success");

				}
				else if (es == ErrorStatus::eKeyNotFound) {
					acutPrintf(L"\nKeyNotFound");

				}
				else {
					acutPrintf(L"\ndelete fail");
				}
				dic->close();
				}
			
			}
		}

	}

	static void AddNamedDic() {
		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		CString dicName = "MyDic";

		if (CSelectUtil::PromptSelectEntity(L"\n��ѡ��ʵ�壺\n", cls, pEnt, pickPoint)) {
		
			pEnt->close();

			AcDbDictionary *pNameDic = NULL;

			acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameDic, AcDb::kForWrite);

			AcDbDictionary *dic = NULL;

			AcDbObjectId dicId;
			
			if (pNameDic->getAt(dicName, dic,AcDb::kForWrite) == ErrorStatus::eKeyNotFound) {

				dic = new AcDbDictionary();

				pNameDic->setAt(dicName, dic, dicId);

				dic->close();
			}
			pNameDic->close();

			if (acdbOpenObject(dic, dicId, AcDb::kForWrite) == ErrorStatus::eOk) {

				AcDbLayerTableRecord *layerRec = new AcDbLayerTableRecord();

				AcCmColor color;
				color.setColorIndex(20);
				layerRec->setColor(color);

				layerRec->setIsOff(true);
				layerRec->setIsFrozen(true);
				
				AcDbObjectId layerId;
				Acad::ErrorStatus es = dic->setAt(dicName, layerRec, layerId);

				layerRec->close();
				dic->close();



			}

		
		
		}
	}

	static void ViewNamedDic() {

		CString dicName = "MyDic";

		AcDbDictionary *pNameDic = NULL;

		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameDic, AcDb::kForWrite);

		AcDbDictionary *dic = NULL;

		AcDbObjectId dicId;


		ErrorStatus es = pNameDic->getAt(dicName, dic);
		if ( es== ErrorStatus::eKeyNotFound) {
			acutPrintf(L"\nfirst KeyNotFound");

		}
		else if (es == ErrorStatus::eOk) {

			AcDbLayerTableRecord *layRec = NULL;

			dic->getAt(dicName, layRec);

			dic->close();
			pNameDic->close();

			if (layRec == NULL) {
				acutPrintf(L"\nsecond KeyNotFound");
			}
			else {

				acutPrintf(L"\n��ɫ��%d", layRec->color().colorIndex());
				acutPrintf(L"\n�Ƿ�ر�:%s", layRec->isOff() ? L"��" : L"��");
				acutPrintf(L"\n�Ƿ񶳽᣺%s", layRec->isFrozen() ? L"��" : L"��");
				layRec->close();

			}
		}
	}

	static void DeleteNamedDic() {

		CString dicName = "MyDic";
		AcDbDictionary *pNameDic = NULL;

		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameDic, AcDb::kForWrite);

		AcDbDictionary *dic = NULL;

		AcDbObjectId dicId;


		ErrorStatus es = pNameDic->getAt(dicName, dic);
		if (es == ErrorStatus::eOk) {

			ErrorStatus es2 = pNameDic->remove(dicName);

			if (es2 == ErrorStatus::eOk) {

				acutPrintf(L"\ndelete success");

			}
			else if (es2 == ErrorStatus::eKeyNotFound) {
				acutPrintf(L"\nKeyNotFound");

			}
			else {
				acutPrintf(L"\ndelete fail");
			}
			dic->close();
		}
		pNameDic->close();
	}

	static void ECDMyGroupMyXData() {
		//����Xdata����
		/*AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\nѡ��ʵ�壺", cls, pEnt, pickPoint)) {

			CString appName = "xDataApp";
			acdbRegApp(appName);

			struct resbuf *rb = acutBuildList(AcDb::kDxfRegAppName, appName,
				AcDb::kDxfXdAsciiString, L"�ַ�����������",
				AcDb::kDxfXdInteger16, 2,
				AcDb::kDxfXdReal, 3.14,
				AcDb::kDxfXdWorldXCoord, asDblArray(pickPoint),
				RTNONE);

			pEnt->setXData(rb);

			acutRelRb(rb);

			acutPrintf(L"\n�����չ���ݳɹ�");

			pEnt->close();

		}*/
		
		//ViewXData();

		//DeleteXData();

	}


	static void ECDMyGroupMyXRec() {

		//AddXDicRecord();
		//ViewXDicRecord();
		//DeleteXDicRecord();
		//AddNamedDic();
		//ViewNamedDic();
		DeleteNamedDic();
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject3App)

ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyXData, MyXData, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyXRec, MyXRec, ACRX_CMD_MODAL, NULL)

