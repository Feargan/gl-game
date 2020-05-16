#include "testobj.h"

CTestObj::CTestObj()
{
	auto cyl = createComponent<CCylinder>();
	cyl->setHeight(0.5);
	cyl->setRadius(0.1);
	
	cyl = createComponent<CCylinder>();
	cyl->setHeight(0.5);
	cyl->setRadius(0.05);
	cyl->setPos(0.0, 0, 0.5);
	cyl->setYaw(90);
}

CTestObj::~CTestObj()
{
}

void CTestObj::renderComponent() const
{
}

void CTestObj::updateComponent()
{
}
