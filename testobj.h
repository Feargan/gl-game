#pragma once
#include "cylinder.h"
class CTestObj :
	public ISceneObject
{
public:
	CTestObj();
	virtual ~CTestObj();

protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;

};

