#pragma once
#include <vector>
class CTextFileUtil
{
public:
	CTextFileUtil();
	~CTextFileUtil();
	// �����ı��ļ�
	static void Save(const TCHAR* fileName, const std::vector<CString> &lines);

	// �����ļ�
	static void Load(const TCHAR* fileName, std::vector<CString> &lines);
};

