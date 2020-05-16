#include <ctime>

#include "scene.h"
#include "cylinder.h"
#include "testobj.h"
#include "surface.h"
#include "cuboid.h"
#include "lantern.h"
#include "trafficcone.h"
#include "car.h"

#include "glwindow.h"


int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
	srand(time(NULL));

	CScene scene;

	auto obj = scene.createObject<CCar>();
	auto surf = scene.createObject<CSurface>(100, 100);

	surf->setDistortion(1.0, 0.0, 0.5, 0.4);
	surf->setColor(0.5, 0.5, 0.5);
	surf->createVertices();
	surf->setPos(0.0, 0.0, -10);
	obj->setPos(0.0, 0.0, -20.0);

	CGlWindow window("Test", 10, 10, 800, 600);
	window.attachScene(scene);
	while (!window.pump())
	{
		obj->setYaw(obj->getYaw() + 1.0);
		surf->setYaw(surf->getYaw() + 1.0);
		window.render();
	}

	return 0;
}
