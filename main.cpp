#include <ctime>

#include "scene.h"
#include "cylinder.h"
#include "surface.h"
#include "cuboid.h"
#include "lantern.h"
#include "trafficcone.h"
#include "car.h"

#include "glwindow.h"


int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
	//srand(time(NULL));
	srand(0);

	PlaySoundA("bumbam.wav", NULL, SND_LOOP | SND_ASYNC);

	CScene scene;

	//auto obj = scene.createObject<CCar>();
	//obj->setPos(0.0, 0.0, -15.0);


	auto surf = scene.createObject<CSurface>(50, 50);
	surf->setDistortion(0.7, 0.3, 0.5, 0.4);
	surf->setColor(0.5, 0.5, 0.5);
	surf->setGradient(0.5, 0.5, 0.5);
	surf->createVertices();
	surf->setPos(0.0, 0.0, -40);
	surf->setPitch(-45);

	

	CGlWindow window("Test", 10, 10, 800, 600);
	window.attachScene(scene);

	std::shared_ptr<ISceneObject> x = surf;
	while (!window.pump())
	{
		//obj->setYaw(obj->getYaw() + 1.0);
		//surf->setYaw(surf->getYaw() + 1.0);
		if (GetAsyncKeyState(VK_UP))
		{
			x->setPos(x->getPos() + CVec3d(0.0, 0.0, 0.5));
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			x->setPos(x->getPos() + CVec3d(0.0, 0.0, -0.5));
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			x->setYaw(x->getYaw() - 1.0);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			x->setYaw(x->getYaw() + 1.0);
		}
		if (GetAsyncKeyState('W'))
		{
			x->setPitch(x->getPitch() + 1.0);
		}
		if (GetAsyncKeyState('S'))
		{
			x->setPitch(x->getPitch() - 1.0);
		}
		window.render();
		//gluLookAt(0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		Sleep(10);
	}

	return 0;
}
