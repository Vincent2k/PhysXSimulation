#include "PxPhysicsAPI.h"

#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

using namespace physx;

class SolarSystem
{
private: 
	PxPhysics* physics;
	PxScene* scene;

public:
	SolarSystem(PxPhysics* physics)
	{
		this->physics = physics;
		physics->getScenes(&scene, 1);
	}
	void advance();
};
#endif