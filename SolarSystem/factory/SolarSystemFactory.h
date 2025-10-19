#include "../entities/SolarSystem.h"
#include "PxPhysicsAPI.h"

#ifndef SOLAR_SYSTEM_FACTORY_H
#define SOLAR_SYSTEM_FACTORY_H

using namespace physx;

namespace Factory
{
	SolarSystem* CreateSolarSystem();
	SolarSystem* CreateSolarSystem(PxPhysics* physics);

	void ReleaseSolarSystem(SolarSystem* solarSystem);
}
#endif