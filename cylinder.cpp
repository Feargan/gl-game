#include "cylinder.h"

#include <cmath>
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CCylinder::CCylinder(CVec3d pos, double height, double radius, CVec3d rotation) : ISceneObject(pos, rotation), m_radius(radius), m_height(height)
{
}

CCylinder::~CCylinder()
{
}

void CCylinder::setRadius(double r)
{
	if(r >= 0.0)
		m_radius = r;
}

double CCylinder::getRadius() const
{
	return m_radius;
}

void CCylinder::setHeight(double h)
{
	if (h >= 0.0)
		m_height = h;
}

double CCylinder::getHeight() const
{
	return m_height;
}

void CCylinder::renderComponent() const
{
	const double diameter = m_radius * 2;
	double x, y, alpha, wysTr = 0.25;
	int i;
	
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.7, 0.7, 0.7);
	glVertex3d(0, 0, -m_height/2);
	for (alpha = 0; alpha <= 2 * M_PI; alpha += M_PI / 20.0)
	{
		glColor3d(0.3, 0.3, 0.3);
		x = m_radius * sin(alpha);
		y = m_radius * cos(alpha);
		glVertex3d(x, y, -m_height/2);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(0.3, 0.3, 0.3);
	for (i = 0; i < m_height / wysTr; i++)
	{
		for (alpha = 0.0; alpha <= 2 * M_PI; alpha += M_PI / 20.0)
		{
			x = m_radius * sin(alpha);
			y = m_radius * cos(alpha);
			//glColor3d(0, ((m_height / wysTr) - i) / (m_height / wysTr), i / (m_height / wysTr));
			glVertex3d(x, y, (i * wysTr) - m_height/2);
			glVertex3d(x, y, (i * wysTr) + wysTr - m_height/2);
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.7, 0.7, 0.7);
	glVertex3d(0, 0, m_height/2);
	for (alpha = 0; alpha >= -2 * M_PI; alpha -= M_PI / 20.0)
	{
		glColor3d(0.3, 0.3, 0.3);
		x = m_radius * sin(alpha);
		y = m_radius * cos(alpha);
		glVertex3d(x, y, m_height/2);
	}
	glEnd();
	glPopMatrix();
}

void CCylinder::updateComponent()
{
}
