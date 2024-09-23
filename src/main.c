	/////////////////////
	// RAYLIB TEMPLATE //
	/////////////////////

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "md_camera.h"
#include "md_moduloid.h"
#include "md_object.h"
#include "md_entities.h"

int main(void) 
{
	
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "raylib - model duck");
		
	SetTargetFPS(60);
	Camera camera = {
		{0.0f, 5.0f, 5.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		60.f,
		CAMERA_PERSPECTIVE
	};

	DisableCursor();


	Duck_Object penger = Create_Duck_Object();
	penger.model = LoadModel("assets/models/penger.obj");
	Entity player = { 0 };

	penger.moduloid[0].id = MODULOID_MOVE;

	penger.moduloid[0].param.reference[0] = (void*)&player;
	while(!WindowShouldClose()) {

		
		Control_Entity(&player, &camera);
		Vector3 offset = Vector3Subtract(camera.position, camera.target);
		camera.target = Vector3Add(player.pos, (Vector3){0.0f,1.0f,0.0f});
		camera.position = Vector3Add(camera.target, offset);
		penger.model.transform = MatrixIdentity();
		Apply_Moduloid_Stack(&penger);
		Pivot_Camera(&camera);

		BeginDrawing();
			/* DRAW HERE */
			ClearBackground(RAYWHITE);
			
			BeginMode3D(camera);
				DrawGrid(10.0f, 1.0f);
				DrawModel(penger.model, Vector3Zero(), 1.0f, WHITE);
			EndMode3D();


		EndDrawing();

	}
	UnloadModel(penger.model);
	return 0;
}
