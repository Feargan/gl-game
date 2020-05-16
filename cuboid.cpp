#include "cuboid.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CCuboid::CCuboid(CVec3d pos, CVec3d size, CVec3d rotation) : ISceneObject(pos, rotation), m_size(size), m_color{1.0, 1.0, 1.0}
{
}

CCuboid::~CCuboid()
{
}

void CCuboid::setSize(double width, double height, double length)
{
	m_size = CVec3d(width, height, length);
}

void CCuboid::setSize(const CVec3d & size)
{
	m_size = size;
}

const CVec3d& CCuboid::getSize() const
{
	return m_size;
}

void CCuboid::setColor(double r, double g, double b)
{
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
}

void CCuboid::renderComponent() const
{
	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(m_color[0], m_color[1], m_color[2]);
	glVertex3d(0, 0, m_size.x);
	glVertex3d(0, 0, 0);
	glVertex3d(m_size.z, 0, m_size.x);

	glVertex3d(m_size.z, 0, 0);
	glVertex3d(m_size.z, m_size.y, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, m_size.y, 0);

	glVertex3d(0, 0, m_size.x);
	glVertex3d(0, m_size.y, m_size.x);
	glVertex3d(m_size.z, 0, m_size.x);
	glVertex3d(m_size.z, m_size.y, m_size.x);
	glVertex3d(m_size.z, 0, 0);
	glVertex3d(m_size.z, m_size.y, 0);

	glVertex3d(m_size.z, m_size.y, m_size.x);
	glVertex3d(0, m_size.y, 0);
	glVertex3d(0, m_size.y, m_size.x);
	glEnd();
}

void CCuboid::updateComponent()
{
}
