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
static bool paused = true;

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

	// Suns
	solarSystem->addPlanet(1, PxTransform(PxVec3(0, 0, 0)), 50);
	solarSystem->addPlanet(2, PxTransform(PxVec3(0, 0, 500)), 50);
	
	solarSystem->addPlanet(3, PxTransform(PxVec3(0, 300, 400)), 15);
	solarSystem->addPlanet(4, PxTransform(PxVec3(200, -300, -400)), 15);
	solarSystem->addPlanet(5, PxTransform(PxVec3(0, -300, 400)), 15);
	solarSystem->addPlanet(6, PxTransform(PxVec3(0, 300, -400)), 15);
	solarSystem->addPlanet(7, PxTransform(PxVec3(200, -100, 600)), 15);
	solarSystem->addPlanet(8, PxTransform(PxVec3(300, 500 , -250)), 15);

	solarSystem->setPlanetMass(1, 5000000);
	solarSystem->setPlanetMass(2, 5000000);
	solarSystem->setPlanetMass(3, 30);
	solarSystem->setPlanetMass(4, 450);
	solarSystem->setPlanetMass(5, 300);
	solarSystem->setPlanetMass(6, 1000);
	solarSystem->setPlanetMass(7, 2000);
	solarSystem->setPlanetMass(8, 400);

	solarSystem->setPlanetLineaireVelocity(1, PxVec3(0, 0, -100));
	solarSystem->setPlanetLineaireVelocity(2, PxVec3(0, 0, -50));

	Rendering::renderPhysX(keyPress, exitCallback);

	return 0;
}
