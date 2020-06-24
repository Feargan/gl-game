#pragma once

#include "sceneobject.h"

class CCylinder :
	public ISceneObject
{
	double m_radius;
	double m_height;
public:
	CCylinder(CScene& scene, CVec3d pos = CVec3d(0.0, 0.0, 0.0), double height = 1.0, double radius = 1.0, CVec3d rotation = CVec3d(0.0, 0.0, 0.0));
	virtual ~CCylinder();

	void setRadius(double r);
	double getRadius() const;

	void setHeight(double h);
	double getHeight() const;
protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
};

