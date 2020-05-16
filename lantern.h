#pragma once

#include "sceneobject.h"

class CLantern :
	public ISceneObject
{
	double m_height;
public:
	CLantern();
	virtual ~CLantern();

	void setHeight(double height);
	double getHeight() const;
protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
};

