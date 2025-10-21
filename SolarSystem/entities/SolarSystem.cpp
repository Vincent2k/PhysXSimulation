#include "SolarSystem.h"
#include "PxPhysicsAPI.h"
#include "Planet.h"
#include <vector>

void SolarSystem::advance()
{
	this->applyGravitationalForces();

	this->scene->simulate(1.0f / 60.0f);
	this->scene->fetchResults(true);
}

void SolarSystem::addPlanet(int id, PxTransform position, PxReal radius)
{
	auto material = this->physics->createMaterial(0.5f, 0.5f, 0.2f);

	PxRigidDynamic* dynamic = PxCreateDynamic(*this->physics, position, PxSphereGeometry(radius), *material, 10.0f);
	dynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	this->scene->addActor(*dynamic);

	auto planet = Planet(dynamic);
	this->planets.insert(std::pair{ id, planet });
}

void SolarSystem::setPlanetMass(int id, float mass)
{
	auto config = this->planets.at(id).getConfigurations();

	this->planets.at(id).setMass(mass);
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
		}

		if (idsToCalculateWith.size() > 0)
		{
			idsToCalculateWith.erase(idsToCalculateWith.begin());
		}
	}
}

PxVec3 SolarSystem::calculateForceBtwPlanets(Planet p1, Planet p2)
{
	return PxVec3(0, 0, 400);
}