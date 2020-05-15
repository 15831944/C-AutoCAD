#include "StdAfx.h"
#include "resource.h"
//���ͷ�ļ��Ϳ�CAD2010������������
#if _MSC_VER == 1500
#include "acplmisc.h"
#pragma comment(lib,"AcPublish.lib")
#else if _MSC_VER > 1500
#include "acplmisc.h"
#pragma comment(lib,"AcPublish_crx.lib")
#endif
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CPublishPrinterApp : public AcRxArxApp {

public:
	CPublishPrinterApp () : AcRxArxApp () {}

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
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CPublishPrinterApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CPublishPrinterApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupPT () {
		// Put your command code here
		/*AcDbDatabase *pDb = new AcDbDatabase();
		pDb->readDwgFile(_T("D:\\TestPublish.dwg"));
		plot(pDb);
		if (pDb != NULL)
		{
			delete pDb;
			pDb = NULL;
		}*/
		plot(acdbHostApplicationServices()->workingDatabase());
		
	}

	
	static void plot(AcDbDatabase *pDb)
	{
		//����pdf����·��
		CString pdfPath = _T("d:\\myPDF.pdf");
		//���ô�ӡ���豸��
		CString deviceName = _T("DWG To PDF.pc3");
		Acad::ErrorStatus es = Acad::eOk;

		const TCHAR *docName;
		//��ȡ���ݵ�·�����ļ���,ע��pdb����ǵ�ǰ�ļ���
		//һ��Ҫ������ٴ�ӡ���������·����������ʱ�ļ�
		//pDb��ʹ�ĺ�̨���ݿ���Ȼ�ܴ�ӡ
		es = pDb->getFilename(docName);

		//��ȡACADӦ�ó��򲼾ֹ�����
		AcDbLayoutManager *layoutManager = acdbHostApplicationServices()->layoutManager();
		AcDbDictionary *layoutDict = NULL;
		//��ȡ���ݿ�Ĳ��ִʵ�
		es = pDb->getLayoutDictionary(layoutDict, AcDb::kForRead);

		//��ʼ�ռ�DSD���ݵ���Ϣ
		// Start collecting info for the DSD data
		AcPlDSDEntries dsdEntries;

		//�������������ִʵ�
		AcDbDictionaryIterator *layoutIterator = layoutDict->newIterator();

		//�������ֻ���ӡͼֽ�ռ������ģ�Ͳ���,��ȡpDbģ�Ϳռ��ID
		// If we only want to plot paper spaces and not the model layout
		AcDbObjectId msId = acdbSymUtil()->blockModelSpaceId(pDb);

		for (; layoutIterator && !layoutIterator->done(); layoutIterator->next())
		{
			AcDbObjectPointer<AcDbLayout> layout(layoutIterator->objectId(), AcDb::kForRead);
			//����ģ�Ϳռ�
			if (layout->getBlockTableRecordId() == msId)
				continue;

			//!!!ע�⣬���ֵĴ�ӡ����Ҫ��ǰ���úã���ֽ�ţ���ӡ���ͣ���
			//����ʹ�õ��Ǳ���Ĳ��ֵ�ҳ�����ã�Ҳ�������ô��ڴ�ӡ�������
			//�˴��Ƕ�ģʽ���ǲ����޸�layout����
			const ACHAR *layoutName;
			layout->getLayoutName(layoutName);
			//���ԣ��޸�����ı�������ǰ׺��pdf��ҳģʽ����Ϊ�ļ���
			CString strPre = _T("sk_");
			strPre += layoutName;
			const ACHAR *szOutLayoutName = (LPCTSTR)strPre;
			//����DSD��Ŀ
			AcPlDSDEntry dsdEntry;

			dsdEntry.setLayout(layoutName);
			dsdEntry.setDwgName(docName);
			//���� �˴��޸�dsdEntry.setTitle(layoutName);
			//��Ŀ���⣬��ҳpdf��ʹ����������������ļ���
			//������Ŀ�ı��������ͬ��ԭ�е���Ŀ���ܻᱻ����
			dsdEntry.setTitle(szOutLayoutName);
			//�����Ŀ��dsd��Ŀ����
			dsdEntries.append(dsdEntry);
		}
		//ɾ��(�ͷ�)���ֱ���������
		delete layoutIterator;
		layoutDict->close();

		//��dsd��Ŀ��ӵ�dsd����
		AcPlDSDData dsdData;
		dsdData.setDSDEntries(dsdEntries);

		//����dsd������Ŀ·��
		dsdData.setProjectPath(_T("d:\\"));
		//����dsd���ݵ���־·����pdf��ӡ��������Ч��û��������־
		dsdData.setLogFilePath(_T("d:\\logdwf.log"));

		//���õ�ҳ���Ƕ�ҳ������ǵ�ҳ������ʹ�ò������������ļ���
		//kSinglePDF kMultiPDF
		dsdData.setSheetType(AcPlDSDEntry::kMultiPDF);
		//���ô�ӡ�ķ���,���pdf��ӡ����������,ʼ����1
		dsdData.setNoOfCopies(1);
		//����Ŀ����,������ļ���(��ҳ��Ч)
		dsdData.setDestinationName(pdfPath);
		//������ʾ����dwf�������Ϊtrue���ܻ���ʾ�����ļ���
		dsdData.setPromptForDwfName(false);
		//����ͼֽ��������,�������ʾ�ڴ�ӡ�ͷ�����ϸ��Ϣ��
		dsdData.setSheetSetName(_T("PublisherSet"));

		//���õ�ǰ���úʹ�ӡ�豸��
		AcPlPlotConfig *plotConfig;
		acplPlotConfigManager->setCurrentConfig(plotConfig, deviceName);


		//�����acplPublishExecute��Ҫ����AcPublish.arx
		// We need this for acplPublishExecute
		acedArxLoad(_T("AcPublish.arx"));

		//������Ҫ#include "acplmisc.h"
		//������Ҫ2010-2012#pragma comment(lib,"AcPublish.lib")
		//������Ҫ2013��������#pragma comment(lib,"AcPublish_crx.lib")
		//ִ�з�������
		acplPublishExecute(dsdData, plotConfig, false);
	}
	

	
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CPublishPrinterApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CPublishPrinterApp, ECDMyGroup, PT, PT, ACRX_CMD_MODAL, NULL)


