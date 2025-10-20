#include "PxPhysicsAPI.h"

#ifndef PLANET_H
#define PLANET_H

using namespace physx;

struct PlanetConfigurations
{
	float mass;
};

class Planet
{
private:
	PxRigidDynamic* rBody;
	PlanetConfigurations configurations;

public:
	Planet(PxRigidDynamic* rBody)
	{
		this->rBody = rBody;
		this->rBody->setMass(0);
		this->configurations.mass = 0;
	}

	const PlanetConfigurations getConfigurations()
	{
		return this->configurations;
	}

	void setMass(float mass)
	{
		this->configurations.mass = mass;
	}

	void applyGravity(Planet& targetPlanet)
	{
		
	}
};

#endif