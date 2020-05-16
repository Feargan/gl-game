#pragma once

#include "sceneobject.h"

constexpr struct
{
	double
		tireRadius = 0.6,
		tire = 0.5,
		lengthTires = 4,
		carFrontExt = 1.5,
		widthTires = 4,
		baseHeight = 2,
		top = 3.5,
		carWidth = widthTires - 0.1;
} g_carPhys;

class CCar :
	public ISceneObject
{
public:
	CCar();
	virtual ~CCar();

protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
};

