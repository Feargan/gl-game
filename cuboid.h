#pragma once

#include "sceneobject.h"

class CCuboid :
	public ISceneObject
{
	CVec3d m_size;

	double m_color[3];
public:
	CCuboid(CVec3d pos = CVec3d(0.0, 0.0, 0.0), CVec3d size = CVec3d(1.0, 1.0, 1.0), CVec3d rotation = CVec3d(0.0, 0.0, 0.0));
	virtual ~CCuboid();

	void setSize(double width, double height, double length);
	void setSize(const CVec3d& size);
	const CVec3d& getSize() const;

	void setColor(double r, double g, double b);
protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
};