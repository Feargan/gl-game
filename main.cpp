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

	double distance = 0;
	constexpr auto& p = g_carPhys;

	srand(0);

	//PlaySoundA("bumbam.wav", NULL, SND_LOOP | SND_ASYNC);
	
	CScene scene;

	struct CamMode
	{
		enum
		{
			BEGIN = 1,
			BACK = BEGIN,
			FRONT,
			FREE_TOP,
			END = FREE_TOP,

		};
	};
	int cameraMode = CamMode::BEGIN;

	auto car = scene.createObject<CCar>();
	car->setPos(5.0, 0.0, -30.0);

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

	auto center = scene.createObject<CCuboid>();
	center->setPos(20, 0, -10);

	auto ri = scene.createObject<CCuboid>();
	ri->setPitch(-90);
	ri->setSize(5, 0.01, 0.01);
	ri->setColor(1.0, 1.0, 0.0);

	auto np = scene.createObject<CCuboid>();
	np->setPitch(-90);
	np->setSize(5, 0.01, 0.01);
	np->setColor(0.0, 1.0, 1.0);

	CGlWindow window("Test", 10, 10, 800, 600);
	window.attachScene(scene);

	auto now = std::chrono::high_resolution_clock::now;
	auto prevFrameTimer = now();
	while (!window.pump())
	{
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
					car->setForce(2000);
					break;
				case 'S':
					car->setForce(-2000.0);
					break;
				case VK_RETURN:
					MessageBoxA(window.getHandle(), "Enter", "test", 0);
					break;
				case 'F':
					cameraMode = (cameraMode%CamMode::END)+1;
					break;
				}
			}
			if (event.type == CWindowEvent::KEY_RELEASE)
			{
				switch (event.keyboard.key)
				{
				case 'W':
				case 'S':
					car->setForce(0.0);
					break;
				}
			}
		}

		/*if (window.getKeyState(VK_UP))
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
		}*/

		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now() - prevFrameTimer).count();
		car->tick(elapsed);
		double steer = car->getSteer();
		const double delta_steer = elapsed*0.04; //0.04
		if(window.getKeyState('A') || window.getKeyState('D'))
		{
			if (!window.getKeyState('A') || !window.getKeyState('D'))
			{
				if (window.getKeyState('A'))
					car->setSteer(steer + delta_steer);
				if (window.getKeyState('D'))
					car->setSteer(steer - delta_steer);
			}
		}
		else
		{
			if (steer > 0.0)
				car->setSteer(steer - delta_steer);
			else if (steer < 0.0)
				car->setSteer(steer + delta_steer);

			if (std::abs(car->getSteer()) <= delta_steer)
				car->setSteer(0.0);
		}

		CVec3d rv = CVec3d(0, 0, -p.lengthTires / tan(car->getSteer() * M_PI / 180)).rotateY(car->getYaw()*M_PI/180);
		ri->setPos(rv + car->getPos());
		CVec3d mt = CVec3d(p.lengthTires, 0.0, 0.0).rotateY(car->getYaw()*M_PI / 180);
		np->setPos((mt - rv).rotateY(distance / (p.lengthTires / sin(car->getSteer() * M_PI / 180))) + car->getPos() + rv);

		//auto dura = std::chrono::duration_cast<std::chrono::milliseconds>(now() - prevFrameTimer);
		// kurde nie potrafie zrobic tego frame limitera x-D
		//if(dura < frameDuration)
		//	std::this_thread::sleep_for(frameDuration-dura);

		double elapsedSecs = elapsed / 1000.0;

		elapsedSecs *= 4;
		switch (cameraMode)
		{
		case CamMode::FRONT:
			scene.follow(car, { 15.0, 2.5, 0 });
			break;
		case CamMode::BACK:
			scene.follow(car, { -15.0, 5.0, 5.0 });
			break;
		case CamMode::FREE_TOP:
			if (window.getKeyState('I'))
				center->setPos(center->getPos() + CVec3d(0, 0, -elapsedSecs));
			if (window.getKeyState('J'))
				center->setPos(center->getPos() + CVec3d{ -elapsedSecs, 0, 0});
			if (window.getKeyState('K'))
				center->setPos(center->getPos() + CVec3d{ 0, 0, elapsedSecs });
			if (window.getKeyState('L'))
				center->setPos(center->getPos() + CVec3d{ elapsedSecs, 0, 0});

			scene.follow(center, { 0, 25, 1 });
			break;
		default:
			scene.follow(car, { 15.0, 2.5, 0 });
		}
		
		window.render();
		prevFrameTimer = now();
	}

	return 0;
}
