#pragma once
#include<vector>
using namespace std;
class CAreaFenGe
{
public:
	CAreaFenGe(ACHAR *fenGeStr,AcDbPolyline *&p,AcDbLine *&l);
	~CAreaFenGe();

public:
	void Command();
private:
	int GetDirection();


private :
	AcDbPolyline *poly;
	AcDbLine *line;
	double totalArea;
	int direction;//1 ���£�2��3���ϣ�4�ϣ�5���ϣ�6�ң�7���£�8��
	vector<double> vecFenGe;
	vector<double>vecArea;
	AcGePoint3d l1Pt, l2Pt;
	AcGeVector3d pyXl;

};

