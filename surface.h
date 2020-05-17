#pragma once

#include "sceneobject.h"
#include "vec3.h"

#include <vector>

class CSurface :
	public ISceneObject
{
	int m_sizeX;
	int m_sizeY;
	double m_distortion[4];
	double m_color[3];
	double m_gradientColor[3];

	std::vector <std::vector<CVec3d>> m_vertices;
public:
	CSurface(int sizeX, int sizeY);
	virtual ~CSurface();

	void setDistortion(double distortionUp, double distortionRight, double distortionDown, double distortionLeft);
	void setColor(double red, double green, double blue);
	void setGradient(double red, double green, double blue);
	void createVertices();
protected:
	virtual void renderComponent() const override;
	virtual void updateComponent() override;
private:
	void putVertexes(int i, int j) const;
};

