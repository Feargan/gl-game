#include "trafficcone.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CTrafficCone::CTrafficCone() : m_height(20.0), m_radius(0.3), m_color{ 1.0,0.7,0.0 }
{
}

CTrafficCone::~CTrafficCone()
{
}

void CTrafficCone::setHeight(double height)
{
	if (height >= 0.0)
		m_height = height;
}

double CTrafficCone::getHeight() const
{
	return m_height;
}

void CTrafficCone::setRadius(double radius)
{
	if (radius >= 0.0)
		m_radius = radius;
}

double CTrafficCone::getRadius() const
{
	return m_radius;
}

void CTrafficCone::renderComponent() const
{
	double alpha, r = m_radius, x, y, h = m_height;

	glColor3d(m_color[0], m_color[1], m_color[2]);

	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(r, r, 0);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(-r, r, 0);
	for (alpha = M_PI / 2; alpha <= M_PI; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(-r, -r, 0);
	for (alpha = M_PI; alpha <= 3 * M_PI / 2; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(r, -r, 0);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();

	//Górne dopelnienie do ko³a
	glColor3d(0.9, 0.3, 0);
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(r, r, r / 10);
	for (alpha = 0.0; alpha <= M_PI / 2; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, r / 10);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(-r, r, r / 10);
	for (alpha = M_PI / 2; alpha <= M_PI; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, r / 10);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(-r, -r, r / 10);
	for (alpha = M_PI; alpha <= 3 * M_PI / 2; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, r / 10);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); //Dope³nienie nad wyciêciem
	glVertex3d(r, -r, r / 10);
	for (alpha = 3 * M_PI / 2; alpha <= 2 * M_PI; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, r / 10);
	}
	glEnd();

	//Dope³nienie wyciêtego ko³a
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(r + r / 10, r + r / 10, 0);
	glVertex3d(r, r + r / 10, 0);
	glVertex3d(r + r / 10, -r - r / 10, 0);
	glVertex3d(r, -r - r / 10, 0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(r, -r, 0);
	glVertex3d(r, -r - r / 10, 0);
	glVertex3d(-r - r / 10, -r, 0);
	glVertex3d(-r - r / 10, -r - r / 10, 0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-r - r / 10, -r, 0);
	glVertex3d(-r, -r, 0);
	glVertex3d(-r - r / 10, r + r / 10, 0);
	glVertex3d(-r, r + r / 10, 0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-r, r + r / 10, 0);
	glVertex3d(-r, r, 0);
	glVertex3d(r + r / 10, r + r / 10, 0);
	glVertex3d(r + r / 10, r, 0);
	glEnd();

	//Dope³nienie wyciêtego ko³a
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(r + r / 10, r + r / 10, r / 10);
	glVertex3d(r, r + r / 10, r / 10);
	glVertex3d(r + r / 10, -r - r / 10, r / 10);
	glVertex3d(r, -r - r / 10, r / 10);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(r, -r, r / 10);
	glVertex3d(r, -r - r / 10, r / 10);
	glVertex3d(-r - r / 10, -r, r / 10);
	glVertex3d(-r - r / 10, -r - r / 10, r / 10);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-r - r / 10, -r, r / 10);
	glVertex3d(-r, -r, r / 10);
	glVertex3d(-r - r / 10, r + r / 10, r / 10);
	glVertex3d(-r, r + r / 10, r / 10);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-r, r + r / 10, r / 10);
	glVertex3d(-r, r, r / 10);
	glVertex3d(r + r / 10, r + r / 10, r / 10);
	glVertex3d(r + r / 10, r, r / 10);
	glEnd();

	//Obwódka
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(r + r / 10, r + r / 10, 0);
	glVertex3d(r + r / 10, r + r / 10, r / 10);
	glVertex3d(r + r / 10, -r - r / 10, 0);
	glVertex3d(r + r / 10, -r - r / 10, r / 10);
	glVertex3d(-r - r / 10, -r - r / 10, 0);
	glVertex3d(-r - r / 10, -r - r / 10, r / 10);
	glVertex3d(-r - r / 10, r + r / 10, 0);
	glVertex3d(-r - r / 10, r + r / 10, r / 10);
	glVertex3d(r + r / 10, r + r / 10, 0);
	glVertex3d(r + r / 10, r + r / 10, r / 10);
	glEnd();

	//SzM_PIc
	for (int j = -10; j < 10; j++) {
		for (int i = -10; i < 10; i++) {
			glBegin(GL_TRIANGLE_FAN);
			glVertex3d(i * 0.01, j * 0.01, 3.0);
			for (alpha = 0; alpha <= 2 * M_PI; alpha += M_PI / 16) {
				x = r * cos(alpha);
				y = r * sin(alpha);
				glVertex3d(x, y, r / 10);
			}
			glEnd();
		}
	}
}

void CTrafficCone::updateComponent()
{
}
