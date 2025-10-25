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
void DrawPlanet(Planet planet, Color color);

using namespace physx;
using namespace std;

static SolarSystem* solarSystem;
static bool paused = true;

void cleanupPhysics(bool /*interactive*/)
{
	Factory::ReleaseSolarSystem(solarSystem);
}

void keyPress(unsigned char key, const PxTransform& camera)
{
	if (key == ' ')
	{
		paused = !paused;
	}
}

void stepPhysics(bool interactive)
{
	if (!paused)
	{
		solarSystem->advance();
	}
}

void exitCallback()
{
	cleanupPhysics(true);
}

void initSolarSystem()
{
	solarSystem = Factory::CreateSolarSystem();

	// Suns
	solarSystem->addPlanet(1, PxTransform(PxVec3(0, 0, 0)), 10);
	solarSystem->addPlanet(2, PxTransform(PxVec3(0, 0, 50)), 10);

	solarSystem->addPlanet(3, PxTransform(PxVec3(0, 30, 30)), 2);
	solarSystem->addPlanet(4, PxTransform(PxVec3(20, -30, -40)), 2);
	solarSystem->addPlanet(5, PxTransform(PxVec3(0, -30, 40)), 2);
	solarSystem->addPlanet(6, PxTransform(PxVec3(0, 30, -40)), 2);
	solarSystem->addPlanet(7, PxTransform(PxVec3(20, -10, 60)), 2);
	solarSystem->addPlanet(8, PxTransform(PxVec3(30, 50, -25)), 2);

	solarSystem->setPlanetMass(1, 500000);
	solarSystem->setPlanetMass(2, 500000);
	solarSystem->setPlanetMass(3, 30);
	solarSystem->setPlanetMass(4, 30);
	solarSystem->setPlanetMass(5, 30);
	solarSystem->setPlanetMass(6, 30);
	solarSystem->setPlanetMass(7, 30);
	solarSystem->setPlanetMass(8, 30);

	solarSystem->setPlanetLinearVelocity(1, PxVec3(0, 0, -0.4));
	solarSystem->setPlanetLinearVelocity(2, PxVec3(0, 0, 0));
}

int main()
{
	initSolarSystem();


	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1900;
	const int screenHeight = 900;

	const float sunRadius = 4.0f;
	const float earthRadius = 0.6f;
	const float earthOrbitRadius = 8.0f;
	const float moonRadius = 0.16f;
	const float moonOrbitRadius = 1.5f;

	InitWindow(screenWidth, screenHeight, "raylib [models] example - rlgl solar system");

	// Define the camera to look into our 3d world
	Camera camera = { 0 };
	camera.position = Vector3{ 104.0f, 69.0f, 36.0f }; // Camera position
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	float rotationSpeed = 0.2f;         // General system rotation speed

	float earthRotation = 0.0f;         // Rotation of earth around itself (days) in degrees
	float earthOrbitRotation = 0.0f;    // Rotation of earth around the Sun (years) in degrees
	float moonRotation = 0.0f;          // Rotation of moon around itself
	float moonOrbitRotation = 0.0f;     // Rotation of moon around earth in degrees

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	int counter = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		stepPhysics(false);
		
		// Update
	   //----------------------------------------------------------------------------------
		UpdateCamera(&camera, CAMERA_FREE);

		if (IsKeyPressed(KEY_Z)) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
		if (IsKeyPressed(KEY_SPACE)) paused = !paused;

		earthRotation += (5.0f * rotationSpeed);
		earthOrbitRotation += (365 / 360.0f * (5.0f * rotationSpeed) * rotationSpeed);
		moonRotation += (2.0f * rotationSpeed);
		moonOrbitRotation += (8.0f * rotationSpeed);
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D(camera);

		DrawPlanet(solarSystem->getPlanet(1), GOLD);
		DrawPlanet(solarSystem->getPlanet(2), GOLD);

		DrawPlanet(solarSystem->getPlanet(3), BLUE);
		DrawPlanet(solarSystem->getPlanet(4), BLUE);
		DrawPlanet(solarSystem->getPlanet(5), BLUE);
		DrawPlanet(solarSystem->getPlanet(6), BLUE);
		DrawPlanet(solarSystem->getPlanet(7), BLUE);
		DrawPlanet(solarSystem->getPlanet(8), BLUE);

		EndMode3D();

		DrawText("EARTH ORBITING AROUND THE SUN!", 400, 10, 20, MAROON);
		DrawFPS(10, 10);

		EndDrawing();
		//----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

	return 0;
}


void DrawPlanet(Planet planet, Color color)
{
	rlPushMatrix();
		rlTranslatef(planet.getGlobalPosition().x, planet.getGlobalPosition().y, planet.getGlobalPosition().z);
		rlScalef(planet.getRadius(), planet.getRadius(), planet.getRadius());
		DrawSphereBasic(color);
	rlPopMatrix();
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
