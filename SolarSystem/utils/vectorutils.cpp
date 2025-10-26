#include "vectorutils.h"

using namespace physx;

namespace utils 
{
	PxVec3 toPxVec3(Vector3 vector3)
	{
		return PxVec3(vector3.x, vector3.y, vector3.z);
	}

	Vector3 toVector3(PxVec3 pxVec)
	{
		return Vector3{ pxVec.x, pxVec.y, pxVec.z };
	}
}