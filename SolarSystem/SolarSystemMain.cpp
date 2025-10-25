// SolarSystem.cpp : définit le point d'entrée de l'application.
//

#include "SolarSystemMain.h"

#include <ctype.h>
#include "PxPhysicsAPI.h"
#include "factory/SolarSystemFactory.h"
#include "entities/SolarSystem.h"
#include "raylib.h"
#include "rlgl.h"
#include <math.h>

void DrawSphereBasic(Color color);
void DrawPlanet(Planet planet);

using namespace physx;
using namespace std;

static SolarSystem* solarSystem;
static bool paused = true;

void stepPhysics(bool interactive)
{
	if (!paused)
	{
		solarSystem->advance();
	}
}

void initSolarSystem()
{
	solarSystem = Factory::CreateSolarSystem();

	// Suns
	solarSystem->addPlanet(1, PxTransform(PxVec3(0, 0, 0)), 10, true);
	solarSystem->addPlanet(2, PxTransform(PxVec3(0, 0, 50)), 10, true);

	solarSystem->addPlanet(3, PxTransform(PxVec3(0, 30, 30)), 2);
	solarSystem->addPlanet(4, PxTransform(PxVec3(20, -30, -40)), 2);
	solarSystem->addPlanet(5, PxTransform(PxVec3(0, -30, 40)), 2);
	solarSystem->addPlanet(6, PxTransform(PxVec3(0, 30, -40)), 2);
	solarSystem->addPlanet(7, PxTransform(PxVec3(20, -10, 60)), 2);
	solarSystem->addPlanet(8, PxTransform(PxVec3(30, 50, -25)), 2);

	solarSystem->setPlanetMass(1, 500000);
	solarSystem->setPlanetMass(2, 5000000);
	solarSystem->setPlanetMass(3, 30);
	solarSystem->setPlanetMass(4, 30);
	solarSystem->setPlanetMass(5, 30);
	solarSystem->setPlanetMass(6, 30);
	solarSystem->setPlanetMass(7, 30);
	solarSystem->setPlanetMass(8, 30);

	solarSystem->setPlanetLinearVelocity(1, PxVec3(0, 0, -20));
	solarSystem->setPlanetLinearVelocity(2, PxVec3(0, 0, 0));
}

int main()
{
	initSolarSystem();

	const int screenWidth = 1900;
	const int screenHeight = 900;

	auto cameraSpeed = 50.0f;

	InitWindow(screenWidth, screenHeight, "raylib [models] example - rlgl solar system");

	Camera camera = { 0 };
	camera.position = Vector3{ 104.0f, 69.0f, 36.0f };
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	int currentGesture = GESTURE_NONE;
	int lastGesture = GESTURE_NONE;
	
	SetTargetFPS(60);

	int counter = 0;
	bool dragging = false;

	auto planets = solarSystem->getPlanets();

    while (!WindowShouldClose())
    {
		lastGesture = currentGesture;
		currentGesture = GetGestureDetected();

		if (currentGesture != lastGesture)
		{
			switch (currentGesture)
			{
				case GESTURE_TAP: dragging = true; break;
				case GESTURE_HOLD: dragging = true; break;
				case GESTURE_DRAG: dragging = true; break;
				case GESTURE_NONE: dragging = false; break;
				default: break;
			}
		}

		stepPhysics(false);
		
		auto previousCameraTarget = camera.target;
		auto previousCameraPosition = camera.position;
		UpdateCamera(&camera, CAMERA_FREE);

		auto xDiff = previousCameraPosition.x - camera.position.x;
		auto yDiff = previousCameraPosition.y - camera.position.y;
		auto zDiff = previousCameraPosition.z - camera.position.z;

		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))
		{
			camera.position = Vector3{ camera.position.x + (xDiff * -cameraSpeed), camera.position.y + (yDiff * -cameraSpeed), camera.position.z + (zDiff * -cameraSpeed) };
		}

		if (!dragging)
		{
			camera.target = previousCameraTarget;
		}
		
		if (IsKeyPressed(KEY_Z)) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
		if (IsKeyPressed(KEY_SPACE)) paused = !paused;
	
		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D(camera);

		for (auto planet : planets)
		{
			DrawPlanet(*planet);
		}

		EndMode3D();

		DrawText("EARTH ORBITING AROUND THE SUN!", 400, 10, 20, MAROON);
		DrawFPS(10, 10);

		EndDrawing();
    }

    CloseWindow();

	Factory::ReleaseSolarSystem(solarSystem);

	return 0;
}


void DrawPlanet(Planet planet)
{
	DrawSphere(
		Vector3{ planet.getGlobalPosition().x, planet.getGlobalPosition().y, planet.getGlobalPosition().z },
		planet.getRadius(),
		planet.isSun() ? GOLD : BLUE
	);
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
// Draw sphere without any matrix transformation
// NOTE: Sphere is drawn in world position ( 0, 0, 0 ) with radius 1.0f
void DrawSphereBasic(Color color)
{
	int rings = 10;
	int slices = 10;

	// Make sure there is enough space in the internal render batch
	// buffer to store all required vertex, batch is reseted if required
	rlCheckRenderBatchLimit((rings + 2) * slices * 6);

	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	for (int i = 0; i < (rings + 2); i++)
	{
		for (int j = 0; j < slices; j++)
		{
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * (j * 360 / slices)));

			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
		}
	}
	rlEnd();
}
