#pragma once

#include "scene.h"

#include <unordered_map>
#include <queue>

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "car.h"
#include "surface.h"
#include "windowevent.h"

class CGlWindow
{
	static constexpr int MOUSE_TIMER = 101;

	static std::unordered_map<HWND, CGlWindow*> m_windows;
	static LRESULT CALLBACK dispatchProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HDC m_hDC;				/* device context */
	HPALETTE m_hPalette = 0;			/* custom palette (if needed) */
	HWND m_hWnd;
	HGLRC m_hRC;				/* opengl context */

	double m_fps;
	CScene* m_scene;

	std::queue<CWindowEvent> m_events;
	bool m_keyStates[255];

	int m_prevMouseX;
	int m_prevMouseY;
public:
	CGlWindow(const char* title, int x, int y, int width, int height, int cmdShow = SW_SHOW);
	~CGlWindow();

	HWND getHandle();

	bool pump();

	bool pollEvent(CWindowEvent& event);
	bool getKeyState(int key) const;


	void attachScene(CScene& scene);
	CScene* getScene();
	void render();
	void setMaxFramerate(double fps);
private:
	HWND create(const char* title, int x, int y, int width, int height, BYTE type, DWORD flags);
	void destroy();
};

