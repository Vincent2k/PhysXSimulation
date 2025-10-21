// SolarSystem.cpp : définit le point d'entrée de l'application.
//

#include "SolarSystemMain.h"

#include <ctype.h>
#include "PxPhysicsAPI.h"
#include "rendering/Rendering.h"
#include "factory/SolarSystemFactory.h"
#include "entities/SolarSystem.h"

using namespace physx;
using namespace std;

static SolarSystem* solarSystem;
static bool paused = false;

void cleanupPhysics(bool /*interactive*/)
{
	Factory::ReleaseSolarSystem(solarSystem);
}

void keyPress(unsigned char key, const PxTransform& camera)
{
	if (key == ' ')
	{
		paused = !paused;
	}
}

void stepPhysics(bool interactive)
{
	if (!paused)
	{
		solarSystem->advance();
	}
}

void exitCallback()
{
	cleanupPhysics(true);
}

int main()
{
	solarSystem = Factory::CreateSolarSystem();
	solarSystem->addPlanet(1, PxTransform(PxVec3(0, 0, 0)), 10);
	solarSystem->addPlanet(2, PxTransform(PxVec3(0, 0, 200)), 10);

	solarSystem->setPlanetMass(1, 20);
	solarSystem->setPlanetMass(2, 40);

	Rendering::renderPhysX(keyPress, exitCallback);

	return 0;
}
