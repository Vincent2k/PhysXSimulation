#include "PxPhysicsAPI.h"
#include "Planet.h"

#include <map>

#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

using namespace physx;

class SolarSystem
{
private: 
	PxPhysics* physics;
	PxScene* scene;
	std::map<int, Planet> planets;
public:
	SolarSystem(PxPhysics* physics)
	{
		this->physics = physics;
		physics->getScenes(&scene, 1);
	}
	void advance();
	void addPlanet(int id, PxTransform position, PxReal radius);

	void setPlanetMass(int id, float mass);
};
#endif