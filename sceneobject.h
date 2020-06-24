#pragma once

#include "vec3.h"
#include "hitsphere.h"
#include "box.h"
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

	std::vector<std::unique_ptr<ISceneObject>> m_children;

	std::vector<CHitSphered> m_refSpheres;
	std::vector<CHitSphered> m_spheres;
	CBoxd m_approxHitbox;
	
	CScene& m_scene;

	bool m_wireframe;
	bool m_stative;
public:
	ISceneObject(CScene& scene, CVec3d pos = CVec3d(0.0, 0.0, 0.0), CVec3d rotation = CVec3d(0.0, 0.0, 0.0), bool stative = false);
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

	void setStative(bool stative);
	bool isStative() const;

	void setWireframe(bool wireframe);

	xstd::observer_ptr<CScene> getScene() const;

	void addColSphere(const CHitSphered& sphere);
	bool against(const ISceneObject& r) const;
	bool checkCollision() const;
	const std::vector<CHitSphered>& getColSpheres() const;
	const CBoxd& getApproximatedHitbox() const;

	void update();
	void render() const;
protected:
	virtual void updateComponent() = 0;
	virtual void renderComponent() const = 0;

	template<typename T, typename ...Cs>
	xstd::observer_ptr<T> createComponent(Cs... args)
	{
		//static_assert(std::is_base_of<ISceneObject, T>::value, "T is not a scene object");
		m_children.emplace_back(std::make_unique<T>(m_scene, args...));
		return static_cast<T*>(m_children.back().get());
	}

	void updateCollision();
};

