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

void cleanupPhysics(bool /*interactive*/)
{
	Factory::ReleaseSolarSystem(solarSystem);
}

void keyPress(unsigned char key, const PxTransform& camera)
{
	
}

void stepPhysics(bool interactive)
{

}

void exitCallback()
{
	cleanupPhysics(true);
}

int main()
{
	solarSystem = Factory::CreateSolarSystem();
	Rendering::renderPhysX(keyPress, exitCallback);
	return 0;
}
