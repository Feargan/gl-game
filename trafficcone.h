#pragma once
#include "sceneobject.h"
class CTrafficCone :
	public ISceneObject
{
	double m_radius;
	double m_height;
	double m_color[3];
public:
	CTrafficCone(CScene& scene);
	virtual ~CTrafficCone();

	void setHeight(double height);
	double getHeight() const;

	void setRadius(double radius);
	double getRadius() const;
protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
};

