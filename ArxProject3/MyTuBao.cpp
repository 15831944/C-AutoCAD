#include "stdafx.h"
#include "MyTuBao.h"
#include"MathUtil.h"
MyTuBao::MyTuBao()
{
}

MyTuBao::MyTuBao(AcGePoint3dArray arr)
{
	this->ptArr = arr;
}


MyTuBao::~MyTuBao()
{
	ptArr.removeAll();

}

void   MyTuBao::GetTuBao(AcGePoint3dArray &pArray)
{
	int nums = 0;
	optimizePointsArray(ptArr);
	nums = ptArr.length();
	if (nums > 3) {
		selectMinPoint(ptArr);//Ѱ����С�ĵ�
		quickSort(ptArr, 1, nums - 1);//�������ݼ��������
		pArray.append(ptArr[0]);
		pArray.append(ptArr[1]);
		pArray.append(ptArr[2]);
		int top = pArray.length() - 1;
		for (int i = 3; i < nums; i++)
		{
			while (xmul(ptArr[i], pArray[top], pArray[top - 1]) >= 0)  //�Ƿ���ת���Ǽ����жϣ����ǣ�����ջ  
			{
				pArray.removeLast();
				top = pArray.length() - 1;
			}
			pArray.append(ptArr[i]);

			top = pArray.length() - 1;
		}

	}
}

void MyTuBao::GetRec(AcGePoint3dArray &pArray){
	selectMinPoint(ptArr);//Ѱ����С�ĵ�
	int nums = 0;
	optimizePointsArray(ptArr);
	nums = ptArr.length();
	quickSort(ptArr, 1, nums - 1);//�������ݼ��������
	pArray = ptArr;

}
void MyTuBao::optimizePointsArray(AcGePoint3dArray &points, double tol /*= 1.0E-7*/) {
	
	for (int i = points.length() - 1; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (CMathUtil::IsEqual(points[i].x, points[j].x, tol) && CMathUtil::IsEqual(points[i].y, points[j].y, tol))
			{
				points.removeAt(i);
				break;
			}
		}
	}
}

long long  MyTuBao::dist(AcGePoint3d p1, AcGePoint3d p2) //��������ƽ��  
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

bool MyTuBao::cmp1(AcGePoint3d p1, AcGePoint3d p2)   //�ҵ���һ���㣬���� true->p1�Ƚ�С��false->p2�Ƚ�С
{
	if (p1.y == p2.y)
		return p1.x<p2.x;
	else
		return p1.y<p2.y;
}

bool MyTuBao::cmp2(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d pointMin)   //��������ıȽϺ���  
{
	if (xmul(p1, p2, pointMin)>0)
		return true; //֤�� p1С ѡ��p1
	else if (xmul(p1, p2, pointMin) == 0 && dist(p1, pointMin)<dist(p2, pointMin))  //��ȵİ��������  
		return true; //֤�� p1С ѡ��p1
	return false;
}

double MyTuBao::xmul(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d  p0)
{
	return (p1.x - p0.x)*(p2.y - p0.y) - (p2.x - p0.x)*(p1.y - p0.y);
}

void MyTuBao::selectMinPoint(AcGePoint3dArray &s) {
	int nums = s.length();
	int cur = 0;
	AcGePoint3d Min = s[0];
	for (int i = 1; i<nums; i++) {
		if (cmp1(s[i], Min)) {
			Min = s[i];
			cur = i;
		}
	}
	s[cur] = s[0];
	s[0] = Min;
	acutPrintf(L"\nѰ�Һ���С��Ϊx=%f,y=%f", s[0].x, s[0].y);
}

void  MyTuBao::quickSort(AcGePoint3dArray &s, int l, int r) {
	if (l< r)
	{
		int i = l, j = r;
		AcGePoint3d x = s[l];
		while (i < j)
		{
			while (i < j && cmp2(x, s[j], s[0])) // ���������ҵ�һ��С��x����
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && cmp2(s[i], x, s[0])) // ���������ҵ�һ�����ڵ���x����
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // �ݹ����
		quickSort(s, i + 1, r);
	}
}
