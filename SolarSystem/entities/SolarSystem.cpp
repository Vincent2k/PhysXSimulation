#include "SolarSystem.h"
#include "PxPhysicsAPI.h"
#include "Planet.h"
#include <vector>
#include <cmath>
#include "../utils/time.h"

const float G_CONSTANT = 30.6674f;

void SolarSystem::advance()
{
	auto ELAPSED_TIME_MS(this->applyGravitationalForces(), this->systemStats.elaspseTimeCalculateForcesMs, test)
	
	ELAPSED_TIME_MS
	(
		this->scene->simulate(1.0f / 60.0f);
		this->scene->fetchResults(true);,
	this->systemStats.elaspseTimeStepMs)
}

void SolarSystem::addPlanet(int id, PxTransform position, PxReal radius, bool isSun)
{
	auto material = this->physics->createMaterial(0.5f, 0.5f, 0.2f);

	PxRigidDynamic* dynamic = PxCreateDynamic(*this->physics, position, PxSphereGeometry(radius), *material, 10.0f);
	dynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	this->scene->addActor(*dynamic);

	auto planet = Planet(dynamic, id, radius, isSun);
	this->planets.insert(std::pair{ id, planet });
	this->appliedForcesByPlanet.insert(std::pair{ id, std::vector<PxVec3>{} });
}

void SolarSystem::setPlanetMass(int id, float mass)
{
	if (this->planets.contains(id))
	{
		this->planets.at(id).setMass(mass);
	}
}

void SolarSystem::setPlanetLinearVelocity(int id, PxVec3 velocity)
{
	if (this->planets.contains(id))
	{
		this->planets.at(id).setLinearVelocity(velocity);
	}
}

void SolarSystem::applyGravitationalForces()
{
	std::vector<int> idsToCalculate;
	std::vector<int> idsToCalculateWith;

	for (const auto &planet : this->planets)
	{
		if (idsToCalculate.size() != 0)
		{
			idsToCalculateWith.push_back(planet.first);
		}
		
		idsToCalculate.push_back(planet.first);
	}

	for (auto planet1It = idsToCalculate.begin(); planet1It != idsToCalculate.end(); planet1It++)
	{
		for (const auto& planet2Id : idsToCalculateWith)
		{
			auto planet1 = this->planets.at(*planet1It);
			auto planet2 = this->planets.at(planet2Id);

			auto force = this->calculateForceBtwPlanets(planet1, planet2);
			auto oppositeforce = PxVec3(-force.x, -force.y, -force.z);

			planet1.applyGravitationalForce(force);
			planet2.applyGravitationalForce(oppositeforce);

			this->appliedForcesByPlanet.at(*planet1It).push_back(force);
			this->appliedForcesByPlanet.at(planet2Id).push_back(oppositeforce);
		}

		if (idsToCalculateWith.size() > 0)
		{
			idsToCalculateWith.erase(idsToCalculateWith.begin());
		}
	}
}

PxVec3 SolarSystem::calculateForceBtwPlanets(Planet p1, Planet p2)
{
	auto massProduct = p1.getMass() * p2.getMass();

	auto positionDiff = p2.getGlobalPosition() - p1.getGlobalPosition();
	auto distance = positionDiff.magnitude();

	auto forceMagnitute = -((G_CONSTANT * massProduct) / (float)pow(distance, 3));
	auto force = positionDiff * forceMagnitute;

	return -force;
}

Planet* SolarSystem::getPlanet(int id)
{
	return &this->planets.at(id);
}

std::vector<Planet*> SolarSystem::getPlanets()
{
	std::vector<Planet*> planets;

	for (auto& planet : this->planets)
	{
		planets.push_back(&planet.second);
	}

	return planets;
}

const SystemStats SolarSystem::getStats()
{
	return this->systemStats;
}