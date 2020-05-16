#include "surface.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

CSurface::CSurface(int sizeX, int sizeY)
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
			m_vertices[k][w].x = w + (m_distortion[1] + m_distortion[3]) * ((double)rand() / (double)RAND_MAX) - m_distortion[3];
			m_vertices[k][w].z = k + (m_distortion[1] + m_distortion[3]) * ((double)rand() / (double)RAND_MAX) - m_distortion[3];
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

void CSurface::renderComponent() const
{
	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(m_color[0], m_color[1], m_color[2]);
	for (int i = 0; i < m_sizeX - 1; i++) {
		if (m_gradientColor[0] != 2.0 && m_gradientColor[1] != 2.0 && m_gradientColor[2] != 2.0)
			glColor3d(	m_color[0] * (1 - i / double(m_sizeX)) + i / double(m_sizeX) * m_gradientColor[0],
						m_color[1] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[1],
						m_color[2] * (1 - i / double(m_sizeX)) + i / (double(m_sizeX)) * m_gradientColor[2]	);
		for (int j = 0; j < m_sizeY; j++) {
			glVertex3d(m_vertices[i][j].x, m_vertices[i][j].y, m_vertices[i][j].z);
			glVertex3d(m_vertices[i + 1][j].x, m_vertices[i + 1][j].y, m_vertices[i + 1][j].z);
		}
	}
	glEnd();
}

void CSurface::updateComponent()
{
}
