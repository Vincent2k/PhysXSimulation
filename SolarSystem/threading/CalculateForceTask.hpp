#include "PxPhysicsAPI.h"

#ifndef CALCULATE_FORCE_TASK_HPP
#define CALCULATE_FORCE_TASK_HPP

class CaculateForcesTask : public physx::PxBaseTask
{
private:
	char* name = NULL;
public:

	CaculateForcesTask() 
	{
	}

	virtual const char* getName() const;
	virtual void run();
};

#endif // CALCULATE_FORCE_TASK_HPP
