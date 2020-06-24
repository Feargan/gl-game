#include "lantern.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CLantern::CLantern() 
	: ISceneObject(CVec3d(0.0, 0.0, 0.0), CVec3d(0.0, 0.0, 0.0), true),
	m_height(15.0)
{
	addColSphere({ 0.1 });
}

CLantern::~CLantern()
{
}

void CLantern::setHeight(double height)
{
	if(m_height >= 0.0)
		m_height = height;
}

double CLantern::getHeight() const
{
	return m_height;
}

void CLantern::renderComponent() const
{
	double alpha, r = 1, x, y;

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.2, 0.2, 0.2);
	glVertex3d(0, 0, 0);
	for (alpha = 0.0; alpha <= 5 / 2 * M_PI; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = r * sin(alpha);
		y = r * cos(alpha);
		glVertex3d(x, y, 0);
		glVertex3d(x, y, 0.2);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, 0);
	for (alpha = 0.0; alpha <= 5 / 2 * M_PI; alpha += M_PI / 16) {
		x = r * cos(alpha);
		y = r * sin(alpha);
		glVertex3d(x, y, 0.2);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= 2 * M_PI; alpha += M_PI / 20.0) {
		x = 0.2 * r * sin(alpha);
		y = 0.2 * r * cos(alpha);
		glVertex3d(x, y, 0.2);
		glVertex3d(x, y, m_height + 0.2);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, m_height + 0.2);
	glVertex3d(1, -1, m_height + 0.2);
	glVertex3d(1, 1, m_height + 0.2);
	glVertex3d(-1, 1, m_height + 0.2);
	glVertex3d(-1, -1, m_height + 0.2);
	glVertex3d(1, -1, m_height + 0.2);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, m_height + 0.2 + 3.5);
	glVertex3d(1.8, -1.8, m_height + 0.2 + 3);
	glVertex3d(1.8, 1.8, m_height + 0.2 + 3);
	glVertex3d(-1.8, 1.8, m_height + 0.2 + 3);
	glVertex3d(-1.8, -1.8, m_height + 0.2 + 3);
	glVertex3d(1.8, -1.8, m_height + 0.2 + 3);
	glEnd();

	//Po³¹czenia u góry
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, -1, m_height + 0.2);
	glVertex3d(1.6, -1.6, m_height + 0.2 + 3);
	glVertex3d(1, -0.9, m_height + 0.2);
	glVertex3d(1.6, -1.5, m_height + 0.2 + 3);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, -1, m_height + 0.2);
	glVertex3d(1.6, -1.6, m_height + 0.2 + 3);
	glVertex3d(0.9, -1, m_height + 0.2);
	glVertex3d(1.5, -1.6, m_height + 0.2 + 3);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, 1, m_height + 0.2);
	glVertex3d(1.6, 1.6, m_height + 0.2 + 3);
	glVertex3d(1, 0.9, m_height + 0.2);
	glVertex3d(1.6, 1.5, m_height + 0.2 + 3);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, 1, m_height + 0.2);
	glVertex3d(1.6, 1.6, m_height + 0.2 + 3);
	glVertex3d(0.9, 1, m_height + 0.2);
	glVertex3d(1.5, 1.6, m_height + 0.2 + 3);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-1, 1, m_height + 0.2);
	glVertex3d(-1.6, 1.6, m_height + 0.2 + 3);
	glVertex3d(-1, 0.9, m_height + 0.2);
	glVertex3d(-1.6, 1.5, m_height + 0.2 + 3);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-1, 1, m_height + 0.2);
	glVertex3d(-1.6, 1.6, m_height + 0.2 + 3);
	glVertex3d(-0.9, 1, m_height + 0.2);
	glVertex3d(-1.5, 1.6, m_height + 0.2 + 3);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-1, -1, m_height + 0.2);
	glVertex3d(-1.6, -1.6, m_height + 0.2 + 3);
	glVertex3d(-1, -0.9, m_height + 0.2);
	glVertex3d(-1.6, -1.5, m_height + 0.2 + 3);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-1, -1, m_height + 0.2);
	glVertex3d(-1.6, -1.6, m_height + 0.2 + 3);
	glVertex3d(-0.9, -1, m_height + 0.2);
	glVertex3d(-1.5, -1.6, m_height + 0.2 + 3);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, -1, m_height + 0.2);
	glVertex3d(1.6, -1.6, m_height + 0.2 + 3);
	glVertex3d(1, -0.9, m_height + 0.2);
	glVertex3d(1.6, -1.5, m_height + 0.2 + 3);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, -1, m_height + 0.2);
	glVertex3d(1.6, -1.6, m_height + 0.2 + 3);
	glVertex3d(0.9, -1, m_height + 0.2);
	glVertex3d(1.5, -1.6, m_height + 0.2 + 3);
	glEnd();

	//Pod i nad œwiat³em
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, 1, m_height + 0.2);
	glVertex3d(1, 1, m_height + 0.2 + 0.2);
	glVertex3d(-1, 1, m_height + 0.2);
	glVertex3d(-1, 1, m_height + 0.2 + 0.2);
	glVertex3d(-1, -1, m_height + 0.2);
	glVertex3d(-1, -1, m_height + 0.2 + 0.2);
	glVertex3d(1, -1, m_height + 0.2);
	glVertex3d(1, -1, m_height + 0.2 + 0.2);
	glVertex3d(1, 1, m_height + 0.2);
	glVertex3d(1, 1, m_height + 0.2 + 0.2);
	glEnd();

	//Œwiat³o
	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(0.5, 0.5, 0);
	glVertex3d(0.99, -0.99, m_height + 0.2);
	glVertex3d(1.59, -1.59, m_height + 0.2 + 3);
	glVertex3d(0.99, 0.99, m_height + 0.2);
	glVertex3d(1.59, 1.59, m_height + 0.2 + 3);
	glVertex3d(-0.99, 0.99, m_height + 0.2);
	glVertex3d(-1.59, 1.59, m_height + 0.2 + 3);
	glVertex3d(-0.99, -0.99, m_height + 0.2);
	glVertex3d(-1.59, -1.59, m_height + 0.2 + 3);
	glVertex3d(0.99, -0.99, m_height + 0.2);
	glVertex3d(1.59, -1.59, m_height + 0.2 + 3);
	glEnd();
}

void CLantern::updateComponent()
{
}
