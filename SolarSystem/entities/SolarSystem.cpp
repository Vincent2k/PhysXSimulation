
#include "SolarSystem.h"

void SolarSystem::advance()
{
	this->scene->simulate(1.0f / 60.0f);
	this->scene->fetchResults(true);
}