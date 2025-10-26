#include "PxPhysicsAPI.h"
#include <vector>

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
	float radius = 1.0f;
	bool sun = false;
	int id;

public:
	Planet(PxRigidDynamic* rBody, int id, float radius, bool isSun)
	{
		this->rBody = rBody;
		this->rBody->setMass(0);
		this->configurations.mass = 0;
		this->radius = radius;
		this->sun = isSun;
		this->id = id;
	}

	const PlanetConfigurations getConfigurations()
	{
		return this->configurations;
	}

	void setMass(float mass)
	{
		this->rBody->setMass(mass);
		this->configurations.mass = mass;
	}

	void applyGravitationalForce(PxVec3 force)
	{
		this->rBody->addForce(force, PxForceMode::eFORCE);
	}

	float getMass() 
	{
		return this->rBody->getMass();
	}

	float getRadius() 
	{
		return this->radius;
	}

	PxVec3 getGlobalPosition() const
	{
		return this->rBody->getGlobalPose().p;
	}

	void setLinearVelocity(PxVec3 velocity)
	{
		this->rBody->setLinearVelocity(velocity);
	}

	bool isSun() const
	{
		return this->sun;
	}

	int getId()
	{
		return this->id;
	}
};

#endif