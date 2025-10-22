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
	float gConstant;

	void applyGravitationalForces();
	PxVec3 calculateForceBtwPlanets(Planet p1, Planet p2);

public:
	SolarSystem(PxPhysics* physics, float gConstant = 0.6674f)
	{
		this->physics = physics;
		this->gConstant = gConstant;
		physics->getScenes(&scene, 1);
	}
	void advance();

	/// <summary>
	/// Add a planet to the Solar System
	/// </summary>
	/// <param name="id">The planet identifier</param>
	/// <param name="position">The planet global position</param>
	/// <param name="radius">The planet radius</param>
	void addPlanet(int id, PxTransform position, PxReal radius);
	void setPlanetMass(int id, float mass);
	void setPlanetLinearVelocity(int id, PxVec3 velicity);
};
#endif