#pragma once

#include <memory>
#include <vector>

#include "sceneobject.h"

class CScene
{
	CVec3d m_camera;
	CVec3d m_lookAt;

	std::vector<std::shared_ptr<ISceneObject>> m_objects;

public:
	CScene();
	~CScene();

	template<typename T, typename ...Cs>
	std::shared_ptr<T> createObject(Cs... args)
	{
		static_assert(std::is_base_of<ISceneObject, T>::value, "T is not a scene object");
		auto newObject = std::make_shared<T>(args...);
		m_objects.push_back(newObject);
		return newObject;
	}

	//void cameraForward();

	void camera() const;
	void follow(std::shared_ptr<ISceneObject>& obj, CVec3d offset = { 0, 0, -5.0 }); // pass by refernce? + template??
	void move(double deep, double horizontal);

	void update();

	void render() const;
};

