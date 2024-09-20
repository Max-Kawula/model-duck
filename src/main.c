	/////////////////////
	// RAYLIB TEMPLATE //
	/////////////////////

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "md_camera.h"
#include "md_moduloid.h"
#include "md_object.h"

int main(void) 
{
	
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "raylib - model duck");
		
	SetTargetFPS(60);
	Camera camera = {
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 1.0f, 0.0f},
		60.f,
		CAMERA_PERSPECTIVE
	};


	Duck_Object penger = Create_Duck_Object();
	penger.model = LoadModel("assets/models/penger.obj");

	Vector3 pos = {1.0f,0.0f,0.0f};
	if((Add_Moduloid(&penger, MODULOID_COPY_LOCATION)) != -1) {
		penger.parameters[0][0] = &pos;
	} else {
		TraceLog(LOG_ERROR, "Failed to add moduloid.");
	}

	// Add_Moduloid(&penger, MODULOID_HOVER);
	// float h_freq = 0.5f;
	// float h_amp = 1.0f;
	// float h_offset = 2.0f;
	// penger.parameters[1][0] = &h_freq;
	// penger.parameters[1][1] = &h_amp;
	// penger.parameters[1][2] = &h_offset;

	/////////////////////
	// GAME LOOP BEGIN //
	/////////////////////
	
	while(!WindowShouldClose()) {
		/* UPDATE LOGIC */	
		Drive_Camera(&camera);
		penger.model.transform = MatrixIdentity();
		Apply_Moduloid_Stack(&penger);

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
