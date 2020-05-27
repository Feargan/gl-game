#include "car.h"
#include "cuboid.h"
#include "cylinder.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

static constexpr double maxAngle = 30.0;

CCar::CCar()
	: m_steerAngle(0.0)
{
	constexpr auto& p = g_carPhys;
	// Kola i osie
	createComponent<CCylinder>(CVec3d(0, 1, 0), p.tire, p.tireRadius - 0.1);
	createComponent<CCylinder>(CVec3d(0, 1, p.widthTires-p.tire), p.tire, p.tireRadius- 0.1);
	m_leftTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, 0), p.tire, p.tireRadius - 0.1);
	m_rightTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, p.widthTires-p.tire), p.tire, p.tireRadius - 0.1);

	createComponent<CCylinder>(CVec3d(0, 1, p.tire), p.widthTires-2*p.tire, p.tireRadius / 6.0);
	createComponent<CCylinder>(CVec3d(p.lengthTires, 1, p.tire), p.widthTires-2*p.tire, p.tireRadius / 6.0);

	// Swiatla
	createComponent<CCuboid>(CVec3d(p.lengthTires + p.carFrontExt, 1.5, 0.4), CVec3d(0.6, 0.3, 0.1));
	createComponent<CCuboid>(CVec3d(p.lengthTires + p.carFrontExt, 1.5, 3.0), CVec3d(0.6, 0.3, 0.1));
	createComponent<CCuboid>(CVec3d(-p.carFrontExt - 0.1, 1.5, 3.0), CVec3d(0.6, 0.3, 0.1));
	createComponent<CCuboid>(CVec3d(-p.carFrontExt - 0.1, 1.5, 0.4), CVec3d(0.6, 0.3, 0.1));

	// Maska
	createComponent<CCuboid>(CVec3d(3.4, p.baseHeight, 0.11), CVec3d(p.carWidth - 0.11, 0.1, p.lengthTires - p.carFrontExt - 0.4))
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

void CCar::forward(double dist)
{
	const double th = getYaw()*M_PI / 180;
	// use in-built  vec operators???
	if (m_steerAngle != 0.0)
	{
		const double a = m_steerAngle / 180 * M_PI;
		const double h = g_carPhys.lengthTires;
		const double r = std::abs(h / sin(a)); // x ^, z >
		const double dx = r * (sin(a + dist / r) - sin(a));
		const double dz = r * (cos(a + dist / r) - cos(a));
		setPos(getPos() + CVec3d{ dz * sin(th) + dx * cos(th), 0,  dz * cos(th) - dx * sin(th) });

		if(m_steerAngle > 0.0)
			setYaw(getYaw() + dist/ (M_PI*r) * 180);
		else
			setYaw(getYaw() - dist / (M_PI*r) * 180);
	}
	/*else if (m_steerAngle < 0.0)
	{
		const double h = g_carPhys.lengthTires;
		const double r = -h / sin(a); // x ^, z >
		const double nx = r * (sin(a + dist / r) - sin(a));
		const double nz = r * (cos(a + dist / r) - cos(a));
		//const double nx = r * (sin(-a - dist / r + M_PI) - sin(-a + M_PI));
		//const double nz = r * (cos(-a - dist / r + M_PI) - cos(-a + M_PI));
		setPos(getPos() + CVec3d{ nz*sin(th) + nx * cos(th), 0, nz*cos(th) - nx * sin(th) });
		//setYaw(getYaw() - dist / (M_PI*r) * 180);
	}*/
	else
		setPos(getPos() + CVec3d{ dist * cos(th), 0, - dist * sin(th) });
}

void CCar::renderComponent() const
{
	constexpr auto& p = g_carPhys;
	double alpha, x, y;

	//Nadkola
	//Lewe tylne ko�o
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(-p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, 0.1);
	}
	glEnd();

	//Prawe tylne ko�o
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(-p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carWidth);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x, y + 1, p.carWidth);
	}
	glEnd();

	//Lewe przednie ko�o
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires - p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, 0.1);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, 0.1);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, 0.1);
	}
	glEnd();

	//Prawe przednie ko�o
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires - p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carWidth);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0, 0.0, 0);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carWidth);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 20.0) {
		x = p.tireRadius * sin(alpha);
		y = p.tireRadius * cos(alpha);
		glVertex3d(x + p.lengthTires, y + 1, p.carWidth);
	}
	glEnd();

	//D� karoserii
	//Lewa strona
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(p.lengthTires + p.tireRadius, 1, 0.1);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, 0.1);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(p.lengthTires, p.baseHeight, 0.1);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(p.lengthTires - p.tireRadius, 1, 0.1);
	glVertex3d(p.lengthTires - p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(p.tireRadius, 1, 0.1);
	glVertex3d(p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(-p.carFrontExt, 1 + p.tireRadius, 0.1);
	glVertex3d(-p.tireRadius, 1, 0.1);
	glVertex3d(-p.carFrontExt, 1, 0.1);
	glVertex3d(-p.tireRadius, p.baseHeight, 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glEnd();
	//Prawa strona
	//D� karoserii od lewej strony
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(p.lengthTires + p.tireRadius, 1, p.carWidth);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(p.lengthTires, p.baseHeight, p.carWidth);
	glVertex3d(p.lengthTires + p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(p.lengthTires - p.tireRadius, 1, p.carWidth);
	glVertex3d(p.lengthTires - p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(p.tireRadius, 1, p.carWidth);
	glVertex3d(p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(p.tireRadius, 1 + p.tireRadius, p.carWidth);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(-p.carFrontExt, 1 + p.tireRadius, p.carWidth);
	glVertex3d(-p.tireRadius, 1, p.carWidth);
	glVertex3d(-p.carFrontExt, 1, p.carWidth);
	glVertex3d(-p.tireRadius, p.baseHeight, p.carWidth);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);
	glEnd();

	//Pokrycie spodu karoserii (g�ra)
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-p.carFrontExt, 1, 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(-p.carFrontExt, 1, p.carWidth);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);

	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, p.baseHeight, 0.1);

	glVertex3d(p.lengthTires + p.carFrontExt, 1, p.carWidth);
	glVertex3d(p.lengthTires + p.carFrontExt, 1, 0.1);
	glEnd();

	//Sp�d nad ko�ami
	//Tylnie lewe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, 0.1);
		glVertex3d(x, y, p.tire + 0.1);
	}
	glEnd();
	//Przednie lewe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.lengthTires + p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, 0.1);
		glVertex3d(x, y, p.tire + 0.1);
	}
	glEnd();
	//Tylnie prawe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carWidth - p.tire);
		glVertex3d(x, y, p.carWidth);
	}
	glEnd();
	//Przednie prawe
	glColor3d(0.1, 0.1, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= M_PI; alpha += M_PI / 16) {
		x = p.lengthTires + p.tireRadius * cos(alpha);
		y = 1 + p.tireRadius * sin(alpha);
		glVertex3d(x, y, p.carWidth - p.tire);
		glVertex3d(x, y, p.carWidth);
	}
	glEnd();

	//Karoseria
	glColor3d(1.0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(0, p.top, 0.2);
	glVertex3d(2, p.baseHeight, 0.1);
	glVertex3d(2.5, p.top, 0.1);
	glVertex3d(3.5, p.baseHeight, 0.1);

	glVertex3d(2.5, p.top, p.carWidth - 0.1);
	glVertex3d(3.5, p.baseHeight, p.carWidth);
	glVertex3d(2.5, p.top, p.carWidth - 0.1);
	glVertex3d(1, p.baseHeight, p.carWidth);
	glVertex3d(0, p.top, p.carWidth - 0.1);
	glVertex3d(-p.carFrontExt, p.baseHeight, p.carWidth);
	glVertex3d(0, p.top, 0.2);
	glVertex3d(-p.carFrontExt, p.baseHeight, 0.1);
	glVertex3d(0, p.top, p.carWidth - 0.1);
	glVertex3d(2.5, p.top, 0.1);
	glVertex3d(2.5, p.top, p.carWidth - 0.1);
	glEnd();
}

void CCar::updateComponent()
{
}
