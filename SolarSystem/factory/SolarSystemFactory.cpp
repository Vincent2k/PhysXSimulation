#include "SolarSystemFactory.h"

namespace Factory
{
	static PxDefaultAllocator		gAllocator;
	static PxDefaultErrorCallback	gErrorCallback;
	static PxFoundation* gFoundation = NULL;
	static PxPhysics* gPhysics = NULL;
	static PxDefaultCpuDispatcher* gDispatcher = NULL;
	static PxScene* gScene = NULL;
	static PxMaterial* gMaterial = NULL;
	static PxPvd* gPvd = NULL;

	SolarSystem* CreateSolarSystem()
	{
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

		gPvd = PxCreatePvd(*gFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.01", 5425, 10);
		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, 0.f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);

		PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		return CreateSolarSystem(gPhysics);
	}

	SolarSystem* CreateSolarSystem(PxPhysics* physics)
	{
		return new SolarSystem(gPhysics);
	}

	void ReleaseSolarSystem(SolarSystem* solarSystem)
	{
		if (gFoundation)
		{
			PX_RELEASE(gScene);
			PX_RELEASE(gDispatcher);
			PX_RELEASE(gPhysics);
			if (gPvd)
			{
				PxPvdTransport* transport = gPvd->getTransport();
				PX_RELEASE(gPvd);
				PX_RELEASE(transport);
			}
			PX_RELEASE(gFoundation);
		}

		delete solarSystem;
		solarSystem = NULL;
	}
}