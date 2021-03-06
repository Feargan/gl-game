#include "sceneobject.h"
#include "scene.h"

#define NOMINMAX
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <algorithm>


ISceneObject::ISceneObject(CScene& scene, CVec3d pos, CVec3d rotation, bool stative)
	: m_pos(pos), m_rotation(rotation), m_scene(scene), m_wireframe(false), m_stative(stative)
{
	m_scene.onStativityChanged(*this);
}

ISceneObject::~ISceneObject()
{
}

xstd::observer_ptr<CScene> ISceneObject::getScene() const
{
	return &m_scene;
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
	if(m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTranslated(m_pos.x, m_pos.y, m_pos.z);
	glRotated(m_rotation.y, 0.0, 1.0, 0.0);
	glRotated(m_rotation.x, 1.0, 0.0, 0.0);
	glRotated(m_rotation.z, 0.0, 0.0, 1.0);
	renderComponent();
	for (auto &p : m_children)
		p->render();
	glPopMatrix();
	if(m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ISceneObject::updateCollision()
{
	if (!m_spheres.empty())
	{

		for (unsigned int i = 0; i < m_refSpheres.size(); i++)
		{
			auto newPos = CVec3d(m_refSpheres[i].pos);
			newPos.rotateY(m_rotation.y / 180 * M_PI);
			newPos.rotateX(m_rotation.x / 180 * M_PI);
			newPos.rotateZ(m_rotation.z / 180 * M_PI);
			m_spheres[i].pos = newPos + m_pos;
		}
	}
	m_scene.onCollisionUpdated(*this);
}

// ... gettery i settery ...

void ISceneObject::setPos(double x, double y, double z)
{
	m_pos = CVec3d(x, y, z);
	updateCollision();
}

void ISceneObject::setPos(const CVec3d & pos)
{
	m_pos = pos;
	updateCollision();
}

const CVec3d& ISceneObject::getPos() const
{
	return m_pos;
}

void ISceneObject::setRotation(double x, double y, double z)
{
	m_rotation = CVec3d(x, y, z);
	updateCollision();
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
	updateCollision();
}

double ISceneObject::getPitch() const
{
	return m_rotation.x;
}

void ISceneObject::setYaw(double yaw)
{
	m_rotation.y = yaw;
	updateCollision();
}

double ISceneObject::getYaw() const
{
	return m_rotation.y;
}

void ISceneObject::setRoll(double roll)
{
	m_rotation.z = roll;
	updateCollision();
}

double ISceneObject::getRoll() const
{
	return m_rotation.z;
}

void ISceneObject::setWireframe(bool wireframe)
{
	m_wireframe = wireframe;
}

void ISceneObject::setStative(bool stative)
{
	if (m_stative != stative)
	{
		m_stative = stative;
		m_scene.onStativityChanged(*this);
	}
}

bool ISceneObject::isStative() const
{
	return m_stative;
}

void ISceneObject::addColSphere(const CHitSphered & sphere)
{
	m_refSpheres.push_back(sphere);
	m_spheres.emplace_back(sphere.r);
	auto maxTop = std::abs(std::max(sphere.pos.x + sphere.r, sphere.pos.z + sphere.r));
	auto minBottom = std::abs(std::min(-sphere.pos.x - sphere.r, -sphere.pos.z - sphere.r));
	auto maxPos = std::max(std::max(maxTop, minBottom), m_approxHitbox.top.x);
	m_approxHitbox.top = { maxPos, 0.0, maxPos };
	m_approxHitbox.bottom = { -maxPos, 0.0, -maxPos };
	updateCollision();
}

bool ISceneObject::against(const ISceneObject & r) const
{
	if (&r == this)
		return false;
	for (auto &ls : m_spheres)
	{
		for (auto &rs : r.m_spheres)
		{
			if (ls.pos.dist(rs.pos) <= ls.r + rs.r)
			{
				return true;
			}
		}
	}
	return false;
}

const CBoxd& ISceneObject::getApproximatedHitbox() const
{
	return m_approxHitbox;
}

bool ISceneObject::checkCollision() const
{
	return m_scene.checkCollision(*this);
}

const std::vector<CHitSphered>& ISceneObject::getColSpheres() const
{
	return m_spheres;
}