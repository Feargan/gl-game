#pragma once

#include "vec3.h"
#include "hitsphere.h"
#include "observer_ptr.h"

#include <memory>
#include <vector>

#ifndef M_PI
constexpr double M_PI = 3.14159265358979323846;
#endif

class CScene;

class ISceneObject
{
	CVec3d m_pos;
	CVec3d m_rotation;

	std::vector<std::shared_ptr<ISceneObject>> m_children;

	std::vector<CHitSphered> m_refSpheres;
	std::vector<CHitSphered> m_spheres;
	
	xstd::observer_ptr<const CScene> m_scene;

	bool m_wireframe;
public:
	ISceneObject(CVec3d pos = CVec3d(0.0, 0.0, 0.0), CVec3d rotation = CVec3d(0.0, 0.0, 0.0));
	virtual ~ISceneObject();

	ISceneObject(const ISceneObject& r);
	ISceneObject(ISceneObject&& r) = default;
	ISceneObject& operator=(const ISceneObject& r);
	ISceneObject& operator=(ISceneObject&& r) = default;

	void setPos(double x, double y, double z);
	void setPos(const CVec3d& pos);
	const CVec3d& getPos() const;

	void setRotation(double x, double y, double z);
	void setRotation(const CVec3d& rotation);
	const CVec3d& getRotation() const;

	void setPitch(double pitch);
	double getPitch() const;

	void setYaw(double yaw);
	double getYaw() const;

	void setRoll(double roll);
	double getRoll() const;

	void setWireframe(bool wireframe);

	void setScene(const CScene& scene);
	xstd::observer_ptr<const CScene> getScene() const;

	void addColSphere(const CHitSphered& sphere);
	bool against(const ISceneObject& r) const;
	bool checkCollision() const;

	void update();
	void render() const;
protected:
	virtual void updateComponent() = 0;
	virtual void renderComponent() const = 0;

	template<typename T, typename ...Cs>
	std::shared_ptr<T> createComponent(Cs... args)
	{
		static_assert(std::is_base_of<ISceneObject, T>::value, "T is not a scene object");
		auto newObject = std::make_shared<T>(args...);
		m_children.push_back(newObject);
		return newObject;
	}

	void updateCollision();
};

