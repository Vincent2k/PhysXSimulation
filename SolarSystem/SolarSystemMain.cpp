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
#include "utils/vectorutils.h"

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
	solarSystem->addPlanet(2000, PxTransform(PxVec3(0, 0, 0)), 5, true);

	solarSystem->setPlanetMass(2000, 5000);
}


int idCount = 100;
void onAddPlanet(Vector3 startPosition) 
{
	solarSystem->addPlanet(++idCount, PxTransform(utils::toPxVec3(startPosition)), 3);
}

#define STAR_COUNT 10000

Vector3 stars[STAR_COUNT] = { 0 };

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
	auto sun = planets.at(0);

	Vector3 lastPosition = {-1, 0, 0};
	Vector3 pointToDraw[100];
	int insertIndex = 0;
	int insertCount = 0;

	// Setup the stars with a random position
	for (int i = 0; i < STAR_COUNT; i++)
	{
		stars[i].x = GetRandomValue(-300, 300);
		stars[i].y = GetRandomValue(-300, 300);
		stars[i].z = GetRandomValue(-300, 300);
	}

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

		if (IsKeyPressed(KEY_F))
		{
			onAddPlanet(camera.position);
			auto planet = solarSystem->getPlanet(idCount);

			auto vel = utils::toPxVec3(camera.target) - utils::toPxVec3(camera.position);
			vel.normalize();
			planet->setLinearVelocity(vel * 20);
			planet->setMass(10);
			planets.push_back(planet);
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

		for (int i = 0; i < STAR_COUNT; i++)
		{
			DrawPoint3D(stars[i], GetRandomValue(0, 1) == 0 ? WHITE : PURPLE);
		}

		for (auto &planet : planets)
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