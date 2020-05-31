#include "car.h"
#include "cuboid.h"
#include "cylinder.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

static constexpr double maxAngle = 30.0;

CCar::CCar()
	: m_steerAngle(0.0), m_force(0.0), m_vel(0.0)
{
	constexpr auto& p = g_carPhys;
	// Kola i osie
	m_tires[0] = createComponent<CCylinder>(CVec3d(0, 1, -p.widthBetweenTires/2 + p.tireWidth / 2), p.tireWidth, p.tireRadius - 0.1);
	m_tires[1] = createComponent<CCylinder>(CVec3d(0, 1, p.widthBetweenTires/2 + p.tireWidth / 2), p.tireWidth, p.tireRadius- 0.1);
	m_tires[2] = m_leftTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, -p.widthBetweenTires/2 + p.tireWidth / 2), p.tireWidth, p.tireRadius - 0.1);
	m_tires[3] = m_rightTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, p.widthBetweenTires/2 + p.tireWidth / 2), p.tireWidth, p.tireRadius - 0.1);

	createComponent<CCylinder>(CVec3d(0, 1, 0), p.widthBetweenTires-2*p.tireWidth, p.tireRadius / 6.0);
	createComponent<CCylinder>(CVec3d(p.lengthTires, 1, 0), p.widthBetweenTires-2*p.tireWidth, p.tireRadius / 6.0);

	// Swiatla
	createComponent<CCuboid>(CVec3d(p.lengthTires + p.carFrontExt, 1.5, p.carLeftSide + 0.3), CVec3d(0.6, 0.3, 0.1));
	createComponent<CCuboid>(CVec3d(p.lengthTires + p.carFrontExt, 1.5, p.carRightSide - 0.3 - 0.6), CVec3d(0.6, 0.3, 0.1));
	createComponent<CCuboid>(CVec3d(-p.carFrontExt - 0.1, 1.5, p.carRightSide - 0.3 - 0.6), CVec3d(0.6, 0.3, 0.1));
	createComponent<CCuboid>(CVec3d(-p.carFrontExt - 0.1, 1.5, p.carLeftSide + 0.3), CVec3d(0.6, 0.3, 0.1));

	// Maska
	createComponent<CCuboid>(CVec3d(3.4, p.baseHeight, p.carLeftSide), CVec3d(abs(p.carLeftSide) + p.carRightSide, 0.1, p.lengthTires - p.carFrontExt - 0.4))
		->setColor(0.2, 0.1, 0.5);
}

CCar::~CCar()
{
}

void CCar::setSteer(double angle)
{
	m_steerAngle = angle;
	if (m_steerAngle > maxAngle)
		m_steerAngle = maxAngle;
	if (m_steerAngle < -maxAngle)
		m_steerAngle = -maxAngle;
	m_leftTire->setYaw(m_steerAngle);
	m_rightTire->setYaw(m_steerAngle);
}

double CCar::getSteer() const
{
	return m_steerAngle;
}

void CCar::setForce(double force)
{
	m_force = force;
}

double CCar::getForce() const
{
	return m_force;
}

void CCar::tick(double elapsed)
{
	constexpr double MAX_VEL = 20.0;
	constexpr double MASS = 500.0;
	const double FRICTION_FACTOR = 0.666;

	const double res = elapsed / 1000;

	m_vel += res * (m_force) / MASS;

	if (m_force*m_vel <= 0)
	{
		m_vel -= m_vel * FRICTION_FACTOR*res;
	}
	if (m_vel > MAX_VEL)
		m_vel = MAX_VEL;
	if (m_vel < -MAX_VEL)
		m_vel = -MAX_VEL;


	forward(m_vel*res);
}

void CCar::forward(double dist)
{
	double th = getYaw()*M_PI / 180;
	if (m_steerAngle != 0.0)
	{
		const double a = m_steerAngle / 180 * M_PI;
		const double h = g_carPhys.lengthTires;
		const double r = std::abs(h / sin(a));
		const double dx = r * (sin(a + dist / r) - sin(a));
		const double dz = r * (cos(a + dist / r) - cos(a));
		setPos(getPos() + CVec3d{ dx, 0.0, dz }.rotateY(th));

		if(m_steerAngle > 0.0)
			setYaw(getYaw() + dist/ (M_PI*r) * 180);
		else
			setYaw(getYaw() - dist / (M_PI*r) * 180);
	}
	else
		setPos(getPos() + CVec3d{ dist * cos(th), 0, - dist * sin(th) });

	const double deltaRot = -dist / g_carPhys.tireRadius * 360.0;
	
	for (auto &p : m_tires)
		p->setRoll(p->getRoll() + deltaRot);
}

void CCar::renderComponent() const
{
	constexpr auto& p = g_carPhys;
	double alpha, x, y;

	//Nadkola
	//Lewe tylne ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(-p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carLeftSide);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carLeftSide);
	}
	glEnd();

	//Prawe tylne ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(-p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carRightSide);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carRightSide);
	}
	glEnd();

	//Lewe przednie ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires - p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carLeftSide);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carLeftSide);
	}
	glEnd();

	//Prawe przednie ko這
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires - p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carRightSide);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carRightSide);
	}
	glEnd();

	//D馧 karoserii
	//Lewa strona
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(p.lengthTires + p.tireRadius, 1, p.carLeftSide);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carLeftSide);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carLeftSide);
	glVertex3d(p.lengthTires, p.baseHeight, p.carLeftSide);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	glVertex3d(p.tireRadius, p.baseHeight, p.carLeftSide);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	glVertex3d(p.lengthTires - p.tireRadius, 1, p.carLeftSide);
	glVertex3d(p.lengthTires - p.tireRadius, p.baseHeight, p.carLeftSide);
	glVertex3d(p.tireRadius, 1, p.carLeftSide);
	glVertex3d(p.tireRadius, p.baseHeight, p.carLeftSide);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carLeftSide);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carLeftSide);
	glVertex3d(-p.carFrontExt, 1 + p.tireRadius, p.carLeftSide);
	glVertex3d(-p.tireRadius, 1, p.carLeftSide);
	glVertex3d(-p.carFrontExt, 1, p.carLeftSide);
	glVertex3d(-p.tireRadius, p.baseHeight, p.carLeftSide);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carLeftSide);
	glEnd();
	//Prawa strona
	//D馧 karoserii od lewej strony
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(p.lengthTires + p.tireRadius, 1, p.carRightSide);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carRightSide);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carRightSide);
	glVertex3d(p.lengthTires, p.baseHeight, p.carRightSide);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	glVertex3d(p.tireRadius, p.baseHeight, p.carRightSide);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	glVertex3d(p.lengthTires - p.tireRadius, 1, p.carRightSide);
	glVertex3d(p.lengthTires - p.tireRadius, p.baseHeight, p.carRightSide);
	glVertex3d(p.tireRadius, 1, p.carRightSide);
	glVertex3d(p.tireRadius, p.baseHeight, p.carRightSide);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carRightSide);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carRightSide);
	glVertex3d(-p.carFrontExt, 1 + p.tireRadius, p.carRightSide);
	glVertex3d(-p.tireRadius, 1, p.carRightSide);
	glVertex3d(-p.carFrontExt, 1, p.carRightSide);
	glVertex3d(-p.tireRadius, p.baseHeight, p.carRightSide);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carRightSide);
	glEnd();

	//Pokrycie spodu karoserii (g鏎a)
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-p.carFrontExt, 1, p.carLeftSide);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carLeftSide);
	glVertex3d(-p.carFrontExt, 1, p.carRightSide);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carRightSide);

	glVertex3d(-p.carFrontExt, p.baseHeight, p.carLeftSide);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carRightSide);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carLeftSide);

	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carRightSide);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carLeftSide);
	glEnd();

	//Sp鏚 nad ko豉mi
	//Tylnie lewe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 20) {
		x = p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carLeftSide);
		glVertex3d(x, y, p.tireWidth + p.carLeftSide);
	}
	glEnd();
	//Przednie lewe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 20) {
		x = p.lengthTires + p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carLeftSide);
		glVertex3d(x, y, p.tireWidth + p.carLeftSide);
	}
	glEnd();
	//Tylnie prawe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 20) {
		x = p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carRightSide - p.tireWidth);
		glVertex3d(x, y, p.carRightSide);
	}
	glEnd();
	//Przednie prawe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 20) {
		x = p.lengthTires + p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carRightSide - p.tireWidth);
		glVertex3d(x, y, p.carRightSide);
	}
	glEnd();

	//Karoseria
	glColor3d(1.0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carLeftSide + p.lean);
	glVertex3d(0, p.top, p.carLeftSide + p.lean);
	glVertex3d(2, p.baseHeight, p.carLeftSide);
	glVertex3d(2.5, p.top, p.carLeftSide + p.lean);
	glVertex3d(3.5, p.baseHeight, p.carLeftSide);

	glVertex3d(2.5, p.top, p.carRightSide - p.lean);
	glVertex3d(3.5, p.baseHeight, p.carRightSide);
	glVertex3d(2.5, p.top, p.carRightSide - p.lean);
	glVertex3d(1, p.baseHeight, p.carRightSide);
	glVertex3d(0, p.top, p.carRightSide - p.lean);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carRightSide);
	glVertex3d(0, p.top, p.carLeftSide + p.lean);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carLeftSide);
	glVertex3d(0, p.top, p.carRightSide - p.lean);
	glVertex3d(2.5, p.top, p.carLeftSide + p.lean);
	glVertex3d(2.5, p.top, p.carRightSide - p.lean);
	glEnd();
}

void CCar::updateComponent()
{
}
