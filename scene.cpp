#include "scene.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CScene::CScene() : m_camera(20.0, 2.0, -10.0), m_lookAt(0.0, 0.0, 0.0)
{
}

CScene::~CScene()
{
}

bool CScene::checkCollision(const ISceneObject & l) const
{
	for (auto &r : m_objects)
	{
		if (l.against(*r))
		{
			return true;
		}
	}
	return false;
}

void CScene::camera() const
{
	gluLookAt(m_camera.x, m_camera.y, m_camera.z, m_lookAt.x, m_lookAt.y, m_lookAt.z, 0.0, 1.0, 0.0);
}

/*void CScene::follow(std::shared_ptr<ISceneObject>& obj, const CVec3d& offset)
{
	m_lookAt = obj->getPos();
	double yaw = obj->getYaw()*M_PI/180;
	double pitch = obj->getPitch()*M_PI/180;
	m_camera = offset;
	m_camera.rotateY(yaw);
	m_camera += m_lookAt;
}*/

void CScene::move(double deep, double horizontal)
{
	m_camera += CVec3d(horizontal, 0.0, deep);
	m_lookAt += CVec3d(horizontal, 0.0, deep);
}

void CScene::update()
{
	for (auto &p : m_objects)
		p->update();
}

void CScene::render() const
{
	for (auto &p : m_objects)
		p->render();
}
