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

	//PlaySoundA("bumbam.wav", NULL, SND_LOOP | SND_ASYNC);

	CScene scene;

	auto obj = scene.createObject<CCar>();
	obj->setPos(5.0, 0.0, -30.0);

	auto surf = scene.createObject<CSurface>(50, 50);
	surf->setDistortion(0.7, 0.3, 0.5, 0.4);
	surf->setColor(0.5, 0.5, 0.5);
	surf->setGradient(0.5, 0.5, 0.5);
	surf->createVertices();
	surf->setPos(0.0, 0.0, -40);

	auto lan1 = scene.createObject<CLantern>();
	lan1->setPos(10.0, 0.0, 5.0);
	lan1->setPitch(-90);

	auto lan2 = scene.createObject<CLantern>();
	lan2->setPos(30.0, 0.0, 5.0);
	lan2->setPitch(-90);

	auto cone1 = scene.createObject<CTrafficCone>();
	cone1->setPos(12.0, 0.0, -30.0);
	cone1->setPitch(-90);

	auto cone2 = scene.createObject<CTrafficCone>();
	cone2->setPos(12.0, 0.0, -25.0);
	cone2->setPitch(-90);

	auto cone3 = scene.createObject<CTrafficCone>();
	cone3->setPos(12.0, 0.0, -20.0);
	cone3->setPitch(-90);

	auto cone4 = scene.createObject<CTrafficCone>();
	cone4->setPos(6.0, 0.0, -20.0);

	CGlWindow window("Test", 10, 10, 800, 600);
	window.attachScene(scene);
	std::shared_ptr<ISceneObject> x = obj;
	while (!window.pump())
	{
		// zamiast asynckeystate zaprogramowac kolejke zdarzen z inputa, ale to potem
		if (GetAsyncKeyState(VK_UP))
		{
			scene.move(0.1, 0.0);
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			scene.move(-0.1, 0.0);
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			scene.move(0.0, -0.1);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			scene.move(0.0, 0.1);
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
		Sleep(10); // prowizorka, uzyc timera w renderze albo cos
	}

	return 0;
}
