#include <ctime>
#include <chrono>
#include <thread>

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
	constexpr double fps = 120.0;
	constexpr std::chrono::duration<double, std::milli> frameDuration(1000/fps);

	//srand(time(NULL));
	srand(0);

	//PlaySoundA("bumbam.wav", NULL, SND_LOOP | SND_ASYNC);

	CScene scene;

	auto obj = scene.createObject<CCar>();
	obj->setPos(5.0, 0.0, -30.0);
	std::shared_ptr<ISceneObject> car = obj;

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

	auto now = std::chrono::high_resolution_clock::now;
	auto prevFrameTimer = now();
	while (!window.pump())
	{
		// zamiast asynckeystate zaprogramowac kolejke zdarzen z inputa, ale to potem
		CWindowEvent event;
		while (window.pollEvent(event))
		{
			if (event.type == CWindowEvent::KEY_PRESS)
			{
				switch (event.keyboard.key)
				{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case 'W':
					obj->setForce(2000);
					break;
				case 'S':
					obj->setForce(-2000.0);
					break;
				case VK_RETURN:
					MessageBoxA(window.getHandle(), "Enter", "test", 0);
				}
			}
			if (event.type == CWindowEvent::KEY_RELEASE)
			{
				switch (event.keyboard.key)
				{
				case 'W':
				case 'S':
					obj->setForce(0.0);
					break;
				}
			}
		}

		if (window.getKeyState(VK_UP))
		{
			scene.move(0.1, 0.0);
		}
		if (window.getKeyState(VK_DOWN))
		{
			scene.move(-0.1, 0.0);
		}
		if (window.getKeyState(VK_LEFT))
		{
			scene.move(0.0, -0.1);
		}
		if (window.getKeyState(VK_RIGHT))
		{
			scene.move(0.0, 0.1);
		}

		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now() - prevFrameTimer).count();
		obj->tick(elapsed);
		double steer = obj->getSteer();
		const double delta_steer = elapsed*0.04;
		if (window.getKeyState('A') || window.getKeyState('D'))
		{
			if (window.getKeyState('A'))
				obj->setSteer(steer + delta_steer);
			else
				obj->setSteer(steer - delta_steer);
		}
		else
		{
			if (steer > 0.0)
				obj->setSteer(steer - delta_steer);
			else if (steer < 0.0)
				obj->setSteer(steer + delta_steer);

			if (std::abs(obj->getSteer()) < 0.1)
				obj->setSteer(0.0);
		}
		
		auto dura = std::chrono::duration_cast<std::chrono::milliseconds>(now() - prevFrameTimer);
		// kurde nie potrafie zrobic tego frame limitera x-D
		//if(dura < frameDuration)
		//	std::this_thread::sleep_for(frameDuration-dura);

		scene.follow(car, { -15.0, 5.0, 5.0 });
		window.render();
		prevFrameTimer = now();
	}

	return 0;
}
