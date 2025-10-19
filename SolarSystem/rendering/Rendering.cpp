#include "Rendering.h"
#include "PxPhysicsAPI.h"
#include "snippetcommon/SnippetPrint.h"
#include "snippetcommon/SnippetPVD.h"
#include "snippetutils/SnippetUtils.h"
#include "snippetrender/SnippetCamera.h"
#include "snippetrender/SnippetRender.h"

using namespace physx;

extern void stepPhysics(bool interactive);

namespace Rendering
{
	Snippets::Camera* sCamera;

	void renderCallback()
	{
		stepPhysics(true);

		Snippets::startRender(sCamera);

		PxScene* scene;
		PxGetPhysics().getScenes(&scene, 1);
		PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors)
		{
			PxArray<PxRigidActor*> actors(nbActors);
			scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			Snippets::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);
		}

		Snippets::finishRender();
	}

	void renderPhysX(KeyboardCallback kbcb, ExitCallback excb)
	{
		sCamera = new Snippets::Camera(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f, -0.2f, -0.7f));

		Snippets::setupDefault("PhysX Solar system", sCamera, kbcb, renderCallback, excb);

		glutMainLoop();

		delete sCamera;
	}
}