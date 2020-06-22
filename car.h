#pragma once

#include <array>

#include "sceneobject.h"

#include "cylinder.h"
#include "cuboid.h"

constexpr struct
{
	double
		tireRadius = 0.6,
		tireWidth = 0.5,
		lengthTires = 4,
		carFrontExt = 1.5,
		widthBetweenTires = 4,
		baseHeight = 2,
		top = 3.5,
		carLeftSide = -widthBetweenTires/2 + 0.1,
		carRightSide = widthBetweenTires/2 - 0.1 + tireWidth,
		lean = 1.3;
} g_carPhys;

class CCar :
	public ISceneObject
{
	double m_steerAngle;
	double m_vel;
	double m_force;

	std::shared_ptr<CCylinder> m_leftTire;
	std::shared_ptr<CCylinder> m_rightTire;
	std::shared_ptr<CCylinder> m_middleTire;
	std::shared_ptr<CCuboid> m_tester;

	std::array<std::shared_ptr<CCylinder>, 4> m_tires;
public:
	CCar();
	virtual ~CCar();

	void setSteer(double angle);
	double getSteer() const;

	void setForce(double force);
	double getForce() const;

	void tick(double elapsed);
protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
private:
	bool forward(double dist);
};

