#pragma once
class CDatabaseJigEntity:public AcDbEntity
{
public:
	CDatabaseJigEntity(const AcDbObjectIdArray & ids) : m_Ids(ids) {}
	~CDatabaseJigEntity();
	CDatabaseJigEntity();
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);//ʵʱ��ʾ�ı�
	void setXform(const AcGeMatrix3d & xform) { m_Xform = xform; }
	bool CDatabaseJigEntity::move();
	BOOL transform();
	BOOL transformedCopy();
private:
	AcDbObjectIdArray	m_Ids;	//���������϶������ID
	AcGeMatrix3d		m_Xform;//�任����
};

