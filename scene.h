#pragma once

#include <memory>
#include <vector>

#include "sceneobject.h"

class CScene
{
	std::vector<std::shared_ptr<ISceneObject>> m_objects;
public:
	template<typename T, typename ...Cs>
	std::shared_ptr<T> createObject(Cs... args)
	{
		static_assert(std::is_base_of<ISceneObject, T>::value, "T is not a scene object");
		auto newObject = std::make_shared<T>(args...);
		m_objects.push_back(newObject);
		return newObject;
	}

	//void cameraForward();

	void update();

	void render() const;

	
	// cameraForward .. 
};

