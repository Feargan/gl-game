#include "sceneobject.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>


ISceneObject::ISceneObject(CVec3d pos, CVec3d rotation) : m_pos(pos), m_rotation(rotation)
{
}

ISceneObject::~ISceneObject()
{
}

void ISceneObject::update()
{
	updateComponent();
	for (auto &p : m_children)
		p->update();
}

void ISceneObject::render() const
{
	glPushMatrix();
	glTranslated(m_pos.x, m_pos.y, m_pos.z);
	glRotated(m_rotation.y, 0.0, 1.0, 0.0);
	glRotated(m_rotation.x, 1.0, 0.0, 0.0);
	glRotated(m_rotation.z, 0.0, 0.0, 1.0);
	renderComponent();
	for (auto &p : m_children)
		p->render();
	glPopMatrix();
}

// ... gettery i settery ...

void ISceneObject::setPos(double x, double y, double z)
{
	m_pos = CVec3d(x, y, z);
}

void ISceneObject::setPos(const CVec3d & pos)
{
	m_pos = pos;
}

const CVec3d& ISceneObject::getPos() const
{
	return m_pos;
}

void ISceneObject::setRotation(double x, double y, double z)
{
	m_rotation = CVec3d(x, y, z);
}

void ISceneObject::setRotation(const CVec3d & rotation)
{
	m_rotation = rotation;
}

const CVec3d & ISceneObject::getRotation() const
{
	return m_rotation;
}

void ISceneObject::setPitch(double pitch)
{
	m_rotation.x = pitch;
}

double ISceneObject::getPitch() const
{
	return m_rotation.x;
}

void ISceneObject::setYaw(double yaw)
{
	m_rotation.y = yaw;
}

double ISceneObject::getYaw() const
{
	return m_rotation.y;
}

void ISceneObject::setRoll(double roll)
{
	m_rotation.z = roll;
}

double ISceneObject::getRoll() const
{
	return m_rotation.z;
}
