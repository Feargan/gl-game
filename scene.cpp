#include "scene.h"
#include "hitsphere.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <algorithm>
#include <array>

CScene::CScene() : m_camera(20.0, 2.0, -10.0), m_lookAt(0.0, 0.0, 0.0), m_sectorsPerEdge(1)
{
	recreateSectors();
}

CScene::~CScene()
{
}

void CScene::camera() const
{
	gluLookAt(m_camera.x, m_camera.y, m_camera.z, m_lookAt.x, m_lookAt.y, m_lookAt.z, 0.0, 1.0, 0.0);
}

void CScene::follow(const ISceneObject& obj, const CVec3d& offset)
{
	m_lookAt = obj.getPos();
	double yaw = obj.getYaw()*M_PI / 180;
	double pitch = obj.getPitch()*M_PI / 180;
	m_camera = offset;
	m_camera.rotateY(yaw);
	m_camera += m_lookAt;
}

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

bool CScene::checkCollision(const ISceneObject & l) const
{
	auto sectors = getObjSectors(l);
	if (!sectors)
		return false;
	auto s = *sectors;

	for (int i = s.sectorxBegin; i <= s.sectorxEnd; i++)
	{
		for (int j = s.sectorzBegin; j <= s.sectorzEnd; j++)
		{
			int ind = m_sectorsPerEdge * i + j;
			if (ind < 0 || static_cast<unsigned int>(ind) >= m_sectors.size())
				continue;
			for (auto &r : m_sectors[ind])
			{
				if (l.against(*r))
				{
					return true;
				}
			}
		}
	}
	return false;
}

CBoxd CScene::getWorldRegion() const
{
	return { m_worldTop, m_worldBottom };
}

void CScene::setWorldRegion(const CVec3d& worldTop, const CVec3d& worldBottom)
{
	m_worldTop = worldTop;
	m_worldBottom = worldBottom;
	m_worldSize = m_worldTop - m_worldBottom;
	recreateSectors();
}

int CScene::getSectorsPerEdge() const
{
	return m_sectorsPerEdge;
}

void CScene::setSectorsPerEdge(int sectorsPerEdge)
{
	if (sectorsPerEdge <= 0)
		return;

	m_sectorsPerEdge = sectorsPerEdge;
	recreateSectors();
}

void CScene::removeStativeObject(const ISceneObject& obj)
{
	for (auto &v : m_sectors)
	{
		v.erase(std::remove_if(v.begin(), v.end(), 
		[&obj](xstd::observer_ptr<const ISceneObject> p)
		{
			return p == &obj;
		}),
		v.end()
		);
	}
}

void CScene::onCollisionUpdated(const ISceneObject& obj)
{
	if (!obj.isStative())
		return;

	removeStativeObject(obj);

	auto sectors = getObjSectors(obj);
	if (!sectors)
		return;
	auto s = *sectors;

	for (int i = s.sectorxBegin; i <= s.sectorxEnd; i++)
	{
		for (int j = s.sectorzBegin; j <= s.sectorzEnd; j++)
		{
			int ind = m_sectorsPerEdge * i + j;
			if(ind >= 0 && static_cast<unsigned int>(ind) < m_sectors.size())
				m_sectors[ind].push_back(&obj);
		}
	}
}

void CScene::onStativityChanged(const ISceneObject& obj)
{
	if (obj.isStative())
		onCollisionUpdated(obj); // +remove from dynamic
	else
		removeStativeObject(obj); // push to dynamic
}

void CScene::recreateSectors()
{
	m_sectors.clear();
	m_sectors.resize(m_sectorsPerEdge*m_sectorsPerEdge);
	for (auto &p : m_objects)
	{
		onCollisionUpdated(*p);
	}
}

std::optional<CScene::CSectorRange> CScene::getObjSectors(const ISceneObject& obj) const
{
	if (obj.getColSpheres().empty())
		return {};
	CSectorRange sectors;
	auto& colSpheres = obj.getColSpheres();
	auto& first = colSpheres[0];

	double 
		top{m_worldTop.z - first.pos.z - first.r}, 
		bottom{m_worldTop.z - first.pos.z + first.r},
		left{m_worldTop.x - first.pos.x - first.r},
		right{m_worldTop.x - first.pos.x + first.r};

	const CVec3d circle;
	for (auto it = ++colSpheres.begin(); it!=colSpheres.end(); ++it)
	{
		auto s = *it;
		CVec3d relTopLeft = m_worldTop - s.pos - CVec3d{ s.r, 0.0, s.r };
		CVec3d relBottomRight = m_worldTop - s.pos + CVec3d{ s.r, 0.0, s.r };

		if (relTopLeft.z < top)
			top = relTopLeft.z;
		if (relBottomRight.z > bottom)
			bottom = relBottomRight.z;
		if (relTopLeft.x < left)
			left = relTopLeft.x;
		if (relBottomRight.x > right)
			right = relBottomRight.x;
	}
	sectors.sectorxBegin = static_cast<int>(m_worldSize.x / right);
	sectors.sectorzBegin = static_cast<int>(m_worldSize.z / bottom);
	sectors.sectorxEnd = static_cast<int>(m_worldSize.x / left);
	sectors.sectorzEnd = static_cast<int>(m_worldSize.z / top);
	return sectors;
}
