#pragma once

#include "scene.h"

#include <unordered_map>

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "car.h"
#include "surface.h"

class CGlWindow
{
	static std::unordered_map<HWND, CGlWindow*> m_windows;
	static LRESULT CALLBACK dispatchProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HDC m_hDC;				/* device context */
	HPALETTE m_hPalette = 0;			/* custom palette (if needed) */
	HWND m_hWnd;
	HGLRC m_hRC;				/* opengl context */

	double m_fps;
	CScene* m_scene;
public:
	CGlWindow(const char* title, int x, int y, int width, int height, int cmdShow = SW_SHOW);
	~CGlWindow();

	void attachScene(CScene& scene);
	CScene* getScene();
	bool pump();
	void render();
	void setMaxFramerate(double fps);
private:
	HWND create(const char* title, int x, int y, int width, int height, BYTE type, DWORD flags);
	void destroy();
};

