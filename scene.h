#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "sceneobject.h"
#include "box.h"

class CScene
{
	CVec3d m_camera;
	CVec3d m_lookAt;

	std::vector<std::unique_ptr<ISceneObject>> m_objects;

	std::vector<std::vector<xstd::observer_ptr <const ISceneObject>>> m_sectors;
	int m_sectorsPerEdge;
	CVec3d m_worldTop;
	CVec3d m_worldBottom;
	CVec3d m_worldSize;

	struct CSectorRange
	{
		int sectorxBegin;
		int sectorxEnd;
		int sectorzBegin;
		int sectorzEnd;
	};
public:
	CScene();
	~CScene();

	template<typename T, typename ...Cs>
	xstd::observer_ptr<T> createObject(Cs... args)
	{
		m_objects.emplace_back(std::make_unique<T>(*this, args...));
		return static_cast<T*>(m_objects.back().get());
	}

	bool checkCollision(const ISceneObject& l) const;

	CBoxd getWorldRegion() const;
	void setWorldRegion(const CVec3d& worldTop, const CVec3d& worldBottom);

	int getSectorsPerEdge() const;
	void setSectorsPerEdge(int sectorsPerEdge);

	//void cameraForward();

	void camera() const;

	void follow(const ISceneObject& obj, const CVec3d& offset = { 0, 0, -5.0 });
	void move(double deep, double horizontal);

	void update();

	void render() const;

	// bind/unbind
	// removeStativeObject
	void onCollisionUpdated(const ISceneObject& obj);
	void onStativityChanged(const ISceneObject& obj);
private:
	void recreateSectors();
	void removeStativeObject(const ISceneObject& obj);

	std::optional<CSectorRange> getObjSectors(const ISceneObject& obj) const;
};

