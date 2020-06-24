#include "car.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <cmath>

static constexpr double maxAngle = 30.0;

CCar::CCar(CScene& scene)
	: ISceneObject(scene), m_steerAngle(0.0), m_force(0.0), m_vel(0.0)
{
	constexpr auto& p = g_carPhys;
	// Kola i osie
	m_tires[0] = createComponent<CCylinder>(CVec3d(0, 1, -p.widthBetweenTires/2 + p.tireWidth / 2), p.tireWidth, p.tireRadius - 0.1);
	m_tires[1] = createComponent<CCylinder>(CVec3d(0, 1, p.widthBetweenTires/2 - p.tireWidth / 2), p.tireWidth, p.tireRadius- 0.1);
	m_tires[2] = m_leftTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, -p.widthBetweenTires/2 + p.tireWidth / 2), p.tireWidth, p.tireRadius - 0.1);
	m_tires[3] = m_rightTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, p.widthBetweenTires/2 - p.tireWidth / 2), p.tireWidth, p.tireRadius - 0.1);

	m_middleTire = createComponent<CCylinder>(CVec3d(p.lengthTires, 1, 0), p.tireWidth, p.tireRadius - 0.1);
	m_tester = createComponent<CCuboid>();
	m_tester->setPitch(-90);
	m_tester->setSize(5, 0.01, 0.01);
	m_tester->setColor(1.0, 0.0, 0.0);

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

	addColSphere(CHitSphered(p.widthBetweenTires / 2, CVec3d(0, 0, 0)));
	addColSphere(CHitSphered(p.widthBetweenTires / 2, CVec3d(p.lengthTires + p.carFrontExt - p.widthBetweenTires / 2, 0, 0)));
	addColSphere(CHitSphered(p.widthBetweenTires / 2, CVec3d((p.lengthTires + p.carFrontExt - p.widthBetweenTires / 2)/2, 0, 0)));

	auto col1 = createComponent<CCylinder>(CVec3d(0, 2.3, 0), 2.3, p.widthBetweenTires / 2);
	auto col2 = createComponent<CCylinder>(CVec3d(p.lengthTires + p.carFrontExt - p.widthBetweenTires / 2, 2.3, 0), 2.3, p.widthBetweenTires / 2);
	auto col3 = createComponent<CCylinder>(CVec3d((p.lengthTires + p.carFrontExt - p.widthBetweenTires / 2) / 2, 2.3, 0), 2.3, p.widthBetweenTires / 2);

	for (auto &c : { col1, col2, col3 })
	{
		c->setPitch(-90);
		c->setWireframe(true);
	}

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
	//m_leftTire->setYaw(m_steerAngle);
	//m_rightTire->setYaw(m_steerAngle);
	m_middleTire->setYaw(m_steerAngle);
	auto radAngle = m_steerAngle * M_PI / 180;
	constexpr auto& p = g_carPhys;
	//m_leftTire->setYaw(1.0/(180.0/m_steerAngle/M_PI - p.widthBetweenTires/p.lengthTires)*180/M_PI);
	//m_rightTire->setYaw(1.0 / (180.0 / m_steerAngle / M_PI - p.widthBetweenTires / p.lengthTires) * 180 / M_PI);
	m_leftTire->setYaw(atan(1.0 / (1.0 / tan(radAngle) - (p.widthBetweenTires - p.tireWidth) / 2.0 / p.lengthTires))*180/M_PI);
	m_rightTire->setYaw(atan(1.0 / (1.0 / tan(radAngle) + (p.widthBetweenTires - p.tireWidth) / 2.0 / p.lengthTires)) * 180 / M_PI);
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
	constexpr double FRICTION_FACTOR = 0.333;

	const double res = elapsed / 1000;

	m_vel += res * (m_force) / MASS;

	if (m_force*m_vel <= 0)
	{
		m_vel -= m_vel * (1.0-FRICTION_FACTOR)*res;
	}
	if (m_vel > MAX_VEL)
		m_vel = MAX_VEL;
	if (m_vel < -MAX_VEL)
		m_vel = -MAX_VEL;


	if (forward(m_vel*res))
		m_vel = 0.0;
}

bool CCar::forward(double dist)
{
	constexpr auto& p = g_carPhys;
	double th = getYaw()*M_PI / 180;
	const double a = m_steerAngle / 180 * M_PI;
	constexpr double h = g_carPhys.lengthTires;
	constexpr double w = g_carPhys.tireWidth;
	const double r = (h / sin(a));
	
	auto oldPos = getPos();
	auto oldYaw = getYaw();

	if (std::abs(m_steerAngle) >= 0.1)
	{
		CVec3d rv = CVec3d(0, 0, -p.lengthTires / tan(m_steerAngle * M_PI / 180)).rotateY(oldYaw*M_PI / 180);
		
		CVec3d mt = CVec3d(m_middleTire->getPos()).rotateY(oldYaw*M_PI / 180);//CVec3d(p.lengthTires, 0.0, 0.0).rotateY(getYaw()*M_PI / 180);
		CVec3d np = (mt - rv).rotateY(dist / (p.lengthTires / sin(m_steerAngle * M_PI / 180))) + oldPos + rv;
		setYaw(oldYaw + 180 / M_PI * dist / (p.lengthTires / sin(m_steerAngle * M_PI / 180)));
		mt = CVec3d(m_middleTire->getPos()).rotateY(getYaw()*M_PI / 180);//CVec3d(p.lengthTires, 0.0, 0.0).rotateY(getYaw()*M_PI / 180);
		//CVec3d mt = CVec3d(p.lengthTires, 0.0, 0.0).rotateY(car->getYaw()*M_PI / 180);
		setPos(np-mt);
	}
	else
		setPos(getPos() + CVec3d{ dist * cos(th), 0, -dist * sin(th) });
	//setYaw(getYaw() + dist / r * 180 / M_PI);
	if (checkCollision())
	{
		setPos(oldPos);
		setYaw(oldYaw);
		return true;
	}

	const double deltaRot = -dist / g_carPhys.tireRadius * 360.0;
	
	for (auto &p : m_tires)
		p->setRoll(p->getRoll() + deltaRot);
	return false;
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
