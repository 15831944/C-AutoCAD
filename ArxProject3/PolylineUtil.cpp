#include "stdafx.h"
#include "PolylineUtil.h"
#include"DwgDataBaseUtil.h"
#include"ConvertUtil.h"
#include"MathUtil.h"
#include"GePointUtil.h"
CPolylineUtil::CPolylineUtil()
{
}


CPolylineUtil::~CPolylineUtil()
{
}
AcDbObjectId  CPolylineUtil::Add(const AcGePoint2dArray &points, double width) {

	int numVerts = points.length();
	AcDbPolyline *pPoly = new AcDbPolyline(numVerts);

	for (int i = 0; i < numVerts; i++)
	{
		pPoly->addVertexAt(i, points.at(i), 0, width, width);
	}
	return  CDwgDataBaseUtil::PostToModelSpace(pPoly, acdbHostApplicationServices()->workingDatabase());


}

AcDbObjectId  CPolylineUtil::Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, double width) {

	AcGePoint2dArray points;
	points.append(ptStart);
	points.append(ptEnd);

	return CPolylineUtil::Add(points, width);
}
AcDbObjectId CPolylineUtil::Add3dPolyline(const AcGePoint3dArray &points)

{

	AcGePoint3dArray verts = points;

	AcDb3dPolyline *pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, verts);

	return CDwgDataBaseUtil::PostToModelSpace(pPoly3d);

}

AcDbObjectId  CPolylineUtil::AddPolygon(const AcGePoint2d &ptCenter, int number, double radius, double rotation, double width)

{

	double angle = 2 * CMathUtil::PI() / (double)number;

	AcGePoint2dArray points;

	for (int i = 0; i < number; i++)

	{

		AcGePoint2d pt;

		pt.x = ptCenter.x + radius*cos(i*angle);

		pt.y = ptCenter.y + radius*sin(i*angle);

		points.append(pt);



	}

	AcDbObjectId polyId = Add(points, width);



	AcDbEntity *pEnt = NULL;

	if (acdbOpenObject(pEnt, polyId, AcDb::kForWrite) == Acad::eOk)

	{

		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);

		if (pPoly != NULL)

		{

			pPoly->setClosed(Adesk::kTrue);

		}

		AcGeVector3d vecNormal = AcGeVector3d(0, 0, 1);



		pEnt->transformBy(AcGeMatrix3d::rotation(rotation, vecNormal, CConvertUtil::ToPoint3d(ptCenter)));

		pEnt->close();

	}

	//EntityUtil::Rotate(polyId, ptCenter, rotation);




	return polyId;

}

AcDbObjectId CPolylineUtil::AddRectangle(const AcGePoint2d &pt1, const AcGePoint2d &pt2, double width)

{



	double x1 = pt1.x, x2 = pt2.x;

	double y1 = pt1.y, y2 = pt2.y;



	AcGePoint2d ptLeftBottom(min(x1, x2), min(y1, y2));

	AcGePoint2d ptRightBottom(max(x1, x2), min(y1, y2));

	AcGePoint2d ptRightTop(max(x1, x2), max(y1, y2));

	AcGePoint2d ptLeftTop(min(x1, x2), max(y1, y2));



	AcDbPolyline *pPoly = new AcDbPolyline(4);

	pPoly->addVertexAt(0, ptLeftBottom, 0, width, width);

	pPoly->addVertexAt(1, ptRightBottom, 0, width, width);

	pPoly->addVertexAt(2, ptRightTop, 0, width, width);

	pPoly->addVertexAt(3, ptLeftTop, 0, width, width);

	pPoly->setClosed(true);

	return CDwgDataBaseUtil::PostToModelSpace(pPoly);



}

AcDbObjectId CPolylineUtil::AddPolyCircle(const AcGePoint2d &ptCenter, double radius, double width)

{

	AcGePoint2d pt1, pt2, pt3;

	pt1.x = ptCenter.x + radius;

	pt1.y = ptCenter.y;

	pt2.x = ptCenter.x - radius;

	pt2.y = ptCenter.y;

	pt3.x = ptCenter.x + radius;

	pt3.y = ptCenter.y;



	AcDbPolyline *pPoly = new AcDbPolyline(3);

	pPoly->addVertexAt(0, pt1, 1, width, width);

	pPoly->addVertexAt(1, pt2, 1, width, width);

	pPoly->addVertexAt(2, pt3, 1, width, width);

	pPoly->setClosed(Adesk::kTrue);



	return CDwgDataBaseUtil::PostToModelSpace(pPoly);

}

bool CPolylineUtil::PointIsPolyVert(AcDbPolyline *pPoly,
	const AcGePoint2d &pt, double tol) {

	for (int i = 0; i < (int)pPoly->numVerts(); i++)
	{
		AcGePoint3d vert;

		pPoly->getPointAt(i, vert);

		if (CGePointUtil::IsEqual(CConvertUtil::ToPoint2d(vert),
			pt, tol)) {
			return true;
		}
	}

	return false;

}

void CPolylineUtil::IntersetWithGeRay(AcDbPolyline * pPoly,
	const AcGeRay2d &geRay,
	AcGePoint3dArray &intPoints, double tol) {

	intPoints.setLogicalLength(0);
	AcGePoint2dArray intPoints2d;

	AcGeTol geTol;
	geTol.setEqualPoint(tol);

	for (int i = 0; i < (int)pPoly->numVerts(); i++)
	{

		if (i < (int)pPoly->numVerts() - 1 || pPoly->isClosed() == Adesk::kTrue) {


			double bulge = 0;
			pPoly->getBulgeAt(i, bulge);

			if (fabs(bulge) < 1.0E-7) {

				AcGeLineSeg2d geLine;
				Acad::ErrorStatus es = pPoly->getLineSegAt(i, geLine);
				AcGePoint2d intPoint;

				if (geLine.intersectWith(geRay, intPoint, geTol) == Adesk::kTrue) {

					if (CGePointUtil::FindPoint(intPoints2d, intPoint, tol) < 0) {


						intPoints2d.append(intPoint);

					}
				}
			}
			else {

				AcGeCircArc2d geArc;
				pPoly->getArcSegAt(i, geArc);
				AcGePoint2d pt1, pt2;
				int count = 0;

				if (geArc.intersectWith(geRay, count, pt1, pt2, geTol) == Adesk::kTrue) {

					if (CGePointUtil::FindPoint(intPoints2d, pt1, tol) < 0) {

						intPoints2d.append(pt1);

					}
					if (count > 1 && CGePointUtil::FindPoint(intPoints2d, pt2, tol) < 0) {

						intPoints2d.append(pt2);

					}
				}
			}
		}
	}
	double z = pPoly->elevation();

	for (int i = 0; i < intPoints2d.length(); i++) {

		intPoints.append(AcGePoint3d(intPoints2d[i].x, intPoints2d[i].y, z));

	}

}

int CPolylineUtil::PtRelationToPoly(AcDbPolyline *pPoly, const AcGePoint2d &pt, double tol) {

	assert(pPoly);

	AcGePoint3d closestPoint;

	pPoly->getClosestPointTo(CConvertUtil::ToPoint3d(pt), closestPoint);

	if (fabs(closestPoint.x - pt.x) < tol&&fabs(closestPoint.y - pt.y) < tol) {
		return 0;
	}

	AcGeVector3d vec(-(closestPoint[X] - pt[X]), -(closestPoint[Y] - pt[Y]), 0);

	AcGeRay2d geRay(AcGePoint2d(pt.x, pt.y), AcGePoint2d(pt.x + vec.x, pt.y + vec.y));

	AcGePoint3dArray intPoints;

	IntersetWithGeRay(pPoly, geRay, intPoints, 1.0E-4);

	CGePointUtil::FilterEqualPoints(intPoints, 1.0E-4);

RETRY:
	if (intPoints.length() == 0) {
		return -1;
	}
	else {
		CGePointUtil::FilterEqualPoints(intPoints,
			CConvertUtil::ToPoint2d(closestPoint));

		for (int i = intPoints.length() - 1; i >= 0; i--) {

			if ((intPoints[i][X] - pt[X])*(closestPoint[X] - pt[X]) >= 0 &&
				(intPoints[i][Y] - pt[Y])*(closestPoint[Y] - pt[Y]) >= 0) {

				intPoints.removeAt(i);

			}
		}

		int count = intPoints.length();
		for (int  i = 0; i < intPoints.length (); i++)
		{

			if (PointIsPolyVert(pPoly, CConvertUtil::ToPoint2d(intPoints[i]), 1.0E-4)) {

				if (PointIsPolyVert(pPoly, AcGePoint2d(pt.x, pt.y), 1.0E-4)) {
					return 0;
				}

				vec = vec.rotateBy(0.035, AcGeVector3d::kZAxis);

				geRay.set(AcGePoint2d(pt.x, pt.y), AcGePoint2d(pt.x + vec.x, pt.y + vec.y));

				intPoints.setLogicalLength(0);
				IntersetWithGeRay(pPoly, geRay, intPoints,1.0E-4);

				goto RETRY;
			}
		}
		if (count % 2 == 0) {

			return -1;
		}
		else {
			return 1;
		}

	}


}