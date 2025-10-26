#include "raylib.h"
#include "PxPhysicsAPI.h"

#ifndef VECTORUTILS_H
#define VECTORUTILS_H

using namespace physx;

namespace utils 
{
	PxVec3 toPxVec3(Vector3 vector3);
	Vector3 toVector3(PxVec3 pxVec);
}

#endif