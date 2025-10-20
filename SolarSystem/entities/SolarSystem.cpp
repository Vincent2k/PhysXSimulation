#include "SolarSystem.h"
#include "PxPhysicsAPI.h"
#include "Planet.h"

void SolarSystem::advance()
{
	this->scene->simulate(1.0f / 60.0f);
	this->scene->fetchResults(true);
}

void SolarSystem::addPlanet(int id, PxTransform position, PxReal radius)
{
	auto material = this->physics->createMaterial(0.5f, 0.5f, 0.6f);
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