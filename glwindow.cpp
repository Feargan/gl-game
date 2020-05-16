/* BASED ON
https://www.opengl.org/archives/resources/code/samples/win32_tutorial/?fbclid=IwAR2L7sC8f9wnFPJCOEL0i_J7Yyc0u0jdwW28t9YkHjwLo3jVyY_X6EeQydE
*/

#include "glwindow.h"


std::unordered_map<HWND, CGlWindow*> CGlWindow::m_windows = {};


CGlWindow::CGlWindow(const char * title, int x, int y, int width, int height, int cmdShow) : m_scene(nullptr)
{
	m_hWnd = create(title, x, y, width, height, PFD_TYPE_RGBA, PFD_DOUBLEBUFFER);
	if (!m_hWnd)
		throw;

	m_hDC = GetDC(m_hWnd);
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

	glEnable(GL_DEPTH_TEST);

	m_windows[m_hWnd] = this;

	ShowWindow(m_hWnd, cmdShow);
}

CGlWindow::~CGlWindow()
{
	m_windows.erase(m_hWnd);
	destroy();
}

void CGlWindow::attachScene(CScene & scene)
{
	m_scene = &scene;
}

CScene* CGlWindow::getScene()
{
	return m_scene;
}

bool CGlWindow::pump()
{
	MSG msg;
	if (GetMessage(&msg, m_hWnd, 0, 0) == 0)
		return true;
	do
	{
		if (msg.message == WM_QUIT)
			return true;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE) == TRUE);
	return false;
}

void CGlWindow::render()
{
	// + timer
	PostMessage(m_hWnd, WM_PAINT, 0, 0);
}

HWND CGlWindow::create(const char* title, int x, int y, int width, int height, BYTE type, DWORD flags)
{
	int         n, pf;
	WNDCLASS    wc;
	LOGPALETTE* lpPal;
	PIXELFORMATDESCRIPTOR pfd;
	static HINSTANCE hInstance = 0;

	/* only register the window class once - use hInstance as a flag. */
	if (!hInstance) {
		hInstance = GetModuleHandle(NULL);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)CGlWindow::dispatchProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "OpenGL";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			return NULL;
		}
	}

	m_hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, width, height, NULL, NULL, hInstance, NULL);

	if (m_hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
			"Error", MB_OK);
		return NULL;
	}

	m_hDC = GetDC(m_hWnd);

	/* there is no guarantee that the contents of the stack that become
	   the pfd are zeroed, therefore _make sure_ to clear these bits. */
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
	pfd.iPixelType = type;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(m_hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", "Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(m_hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			"Cannot set format specified.", "Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(m_hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE ||
		pfd.iPixelType == PFD_TYPE_COLORINDEX) {

		n = 1 << pfd.cColorBits;
		if (n > 256) n = 256;

		lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
			sizeof(PALETTEENTRY) * n);
		memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = n;

		GetSystemPaletteEntries(m_hDC, 0, n, &lpPal->palPalEntry[0]);

		/* if the pixel type is RGBA, then we want to make an RGB ramp,
		   otherwise (color index) set individual colors. */
		if (pfd.iPixelType == PFD_TYPE_RGBA) {
			int redMask = (1 << pfd.cRedBits) - 1;
			int greenMask = (1 << pfd.cGreenBits) - 1;
			int blueMask = (1 << pfd.cBlueBits) - 1;
			int i;

			/* fill in the entries with an RGB color ramp. */
			for (i = 0; i < n; ++i) {
				lpPal->palPalEntry[i].peRed =
					(((i >> pfd.cRedShift)   & redMask) * 255) / redMask;
				lpPal->palPalEntry[i].peGreen =
					(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
				lpPal->palPalEntry[i].peBlue =
					(((i >> pfd.cBlueShift)  & blueMask) * 255) / blueMask;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}
		else {
			lpPal->palPalEntry[0].peRed = 0;
			lpPal->palPalEntry[0].peGreen = 0;
			lpPal->palPalEntry[0].peBlue = 0;
			lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
			lpPal->palPalEntry[1].peRed = 255;
			lpPal->palPalEntry[1].peGreen = 0;
			lpPal->palPalEntry[1].peBlue = 0;
			lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
			lpPal->palPalEntry[2].peRed = 0;
			lpPal->palPalEntry[2].peGreen = 255;
			lpPal->palPalEntry[2].peBlue = 0;
			lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
			lpPal->palPalEntry[3].peRed = 0;
			lpPal->palPalEntry[3].peGreen = 0;
			lpPal->palPalEntry[3].peBlue = 255;
			lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
		}

		m_hPalette = CreatePalette(lpPal);
		if (m_hPalette) {
			SelectPalette(m_hDC, m_hPalette, FALSE);
			RealizePalette(m_hDC);
		}

		free(lpPal);
	}

	ReleaseDC(m_hWnd, m_hDC);

	return m_hWnd;
}

void CGlWindow::destroy()
{
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(m_hWnd, m_hDC);
	wglDeleteContext(m_hRC);
	DestroyWindow(m_hWnd);
	if (m_hPalette)
		DeleteObject(m_hPalette);
}

LRESULT CGlWindow::dispatchProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_windows.find(hWnd) == m_windows.end())
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	return m_windows[hWnd]->windowProc(uMsg, wParam, lParam);
}

LRESULT CGlWindow::windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (uMsg) {
	/*
	uwaga, nie da sie obsluzyc WM_CREATE
	*/
	case WM_TIMER:
		// mozna dodac addTimer czy cos
		/*switch (wParam)
		{
		case 101:
			obj->setYaw(obj->getYaw() + 1.0);
			surf->setYaw(surf->getYaw() + 1.0);
			
			break;
		}*/
		break;
	case WM_PAINT:
		if (!m_scene)
			return 0;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_scene->render();
		glFlush();
		SwapBuffers(m_hDC);
		BeginPaint(m_hWnd, &ps);
		EndPaint(m_hWnd, &ps);
		return 0;
	case WM_SIZE:
		{
			///reshape(LOWORD(lParam), HIWORD(lParam));
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60.0, (float)width / height, 0.001, 100.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -3.0f);
			PostMessage(m_hWnd, WM_PAINT, 0, 0);
		}
		return 0;

	case WM_CHAR:
		switch (wParam) {
		case 27:			/* ESC key */
			PostQuitMessage(0);
			break;
		}
		return 0;

	/*case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetCapture(m_hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (uMsg == WM_LBUTTONDOWN)
			state |= PAN;
		if (uMsg == WM_RBUTTONDOWN)
			state |= ROTATE;
		return 0;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		state = 0;
		return 0;

	case WM_MOUSEMOVE:
		if (state) {
			omx = mx;
			omy = my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			if (mx & 1 << 15) mx -= (1 << 16);
			if (my & 1 << 15) my -= (1 << 16);
			update(state, omx, mx, omy, my);
			PostMessage(m_hWnd, WM_PAINT, 0, 0);
		}
		return 0;*/

	case WM_PALETTECHANGED:
		if (m_hWnd == (HWND)wParam)
			break;
		/* fall through to WM_QUERYNEWPALETTE */

	case WM_QUERYNEWPALETTE:
		if (m_hPalette) {
			UnrealizeObject(m_hPalette);
			SelectPalette(m_hDC, m_hPalette, FALSE);
			RealizePalette(m_hDC);
			return TRUE;
		}
		return FALSE;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}