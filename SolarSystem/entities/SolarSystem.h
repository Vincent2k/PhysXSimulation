#include "PxPhysicsAPI.h"
#include "Planet.h"
#include <map>
#include <vector>

#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

using namespace physx;

struct SystemStats
{
	long long elaspseTimeStepMs = 0;
	long long elaspseTimeCalculateForcesMs = 0;
};

class SolarSystem
{
private: 
	PxPhysics* physics;
	PxScene* scene;
	std::map<int, Planet> planets;
	std::map<int, std::vector<PxVec3>> appliedForcesByPlanet;
	float gConstant;
	SystemStats systemStats;

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
	void addPlanet(int id, PxTransform position, PxReal radius, bool isSun = false);
	void setPlanetMass(int id, float mass);
	void setPlanetLinearVelocity(int id, PxVec3 velicity);

	Planet* getPlanet(int id);
	std::vector<Planet*> getPlanets();
	const SystemStats getStats();
};
#endif