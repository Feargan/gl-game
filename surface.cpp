#include "surface.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CSurface::CSurface(int sizeX, int sizeY)
	: ISceneObject(CVec3d(0.0, 0.0, 0.0), CVec3d(0.0, 0.0, 0.0), true)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	std::fill(m_distortion, m_distortion + 4, 0);
	std::fill(m_color, m_color + 3, 1);
	std::fill(m_gradientColor, m_gradientColor + 3, 2.0);

	m_vertices.resize(sizeY);
	for (int i = 0; i < sizeY; i++)
	{
		m_vertices[i].resize(sizeX);
	}

	createVertices();
}

CSurface::~CSurface()
{
}

void CSurface::setDistortion(double distortionUp, double distortionRight, double distortionDown, double distortionLeft)
{
	m_distortion[0] = distortionUp;
	m_distortion[1] = distortionRight;
	m_distortion[2] = distortionDown;
	m_distortion[3] = distortionLeft;
}

void CSurface::createVertices()
{
	for (int k = 0; k < m_sizeX; k++)
	{
		for (int w = 0; w < m_sizeY; w++)
		{
			m_vertices[k][w].x = w + (m_distortion[1] + m_distortion[3]) * ((double)rand() / (double)RAND_MAX) - m_distortion[1];
			m_vertices[k][w].z = k + (m_distortion[1] + m_distortion[3]) * ((double)rand() / (double)RAND_MAX) + m_distortion[3];
			m_vertices[k][w].y = (m_distortion[0] + m_distortion[2]) * ((double)rand() / (double)RAND_MAX) - m_distortion[2];
		}
	}
}

void CSurface::setColor(double red, double green, double blue)
{
	m_color[0] = red;
	m_color[1] = green;
	m_color[2] = blue;
}

void CSurface::setGradient(double red, double green, double blue)
{
	m_gradientColor[0] = red;
	m_gradientColor[1] = green;
	m_gradientColor[2] = blue;
}

void CSurface::renderComponent() const
{
	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(m_color[0], m_color[1], m_color[2]);
	for (int i = 0; i < m_sizeX - 1; i++) {
		//if (m_gradientColor[0] != 2.0 && m_gradientColor[1] != 2.0 && m_gradientColor[2] != 2.0)
	/*glColor3d(	m_color[0] * (1 - i / double(m_sizeX)) + i / double(m_sizeX) * m_gradientColor[0],
				m_color[1] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[1],
				m_color[2] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[2]	);*/
		for (int j = 0; j < m_sizeY; j++) {
			putVertexes(i, j);
		}
		i++;
		if (i >= m_sizeX - 1)
			break;
		for (int j = m_sizeY-1; j >= 0; j--) {
			putVertexes(i, j);
		}
	}
	glEnd();
}

void CSurface::updateComponent()
{
}

void CSurface::putVertexes(int i, int j) const
{
	/*if (m_vertices[i][j].y > 0.5 || m_vertices[i + 1][j].y > 0.5)
	{
		glColor3d(m_color[0] * (1 - i / double(m_sizeX)) + i / double(m_sizeX) * m_gradientColor[0] * 0.1,
			m_color[1] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[1] * 0.1,
			m_color[2] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[2] * 0.1);
	}
	else
	{
		glColor3d(m_color[0] * (1 - i / double(m_sizeX)) + i / double(m_sizeX) * m_gradientColor[0],
			m_color[1] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[1],
			m_color[2] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[2]);
	}*/
	glColor3d(m_color[0] * (1 - i / double(m_sizeX)) + i / double(m_sizeX) * m_gradientColor[0] * m_vertices[i][j].y,
		m_color[1] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[1]*m_vertices[i][j].y,
		m_color[2] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[2] * m_vertices[i][j].y);
	//glColor3d((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
	glVertex3d(m_vertices[i][j].x, m_vertices[i][j].y, m_vertices[i][j].z);
	glColor3d(m_color[0] * (1 - i / double(m_sizeX)) + i / double(m_sizeX) * m_gradientColor[0] * m_vertices[i+1][j].y,
		m_color[1] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[1] * m_vertices[i+1][j].y,
		m_color[2] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[2] * m_vertices[i+1][j].y);
	glVertex3d(m_vertices[i + 1][j].x, m_vertices[i + 1][j].y, m_vertices[i + 1][j].z);
}
