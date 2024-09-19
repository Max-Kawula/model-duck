#ifndef MD_CAMERA_H
#define MD_CAMERA_H

#include "raylib.h"
#include "raymath.h"

#define CAM_SPEED (2.00f)
#define ORBIT_SPEED (0.01f)
#define SCROLL_SPEED (0.10f)

/* Default Camera State */
#define ORIGIN ((Vector3){0.0f,0.0f,0.0f})
#define UP ((Vector3){0.0f,1.0f,0.0f})
#define DOWN ((Vector3){0.0f,-1.0f,0.0f})
#define FOV 60.0f
#define INITIAL_POS ((Vector3){0.0f,2.5f,3.0f})

void Drive_Camera(Camera* camera)
{
	Vector3 translate = Vector3Zero();

	Vector2 key_dir = Vector2Zero();
	Vector2 mouse_dir = Vector2Zero();
	float wheel_mag = SCROLL_SPEED*GetMouseWheelMove();

	Vector3 offset = Vector3Subtract(camera->position, camera->target);
	Vector3 basis_x = Vector3Normalize(Vector3CrossProduct(offset, camera->up)); // not normalized!
	float camera_yaw = -atan2f(offset.x, offset.z);
	float camera_scale = Vector3Length(offset);
	float delta_time = GetFrameTime();

	/* TRANSLATION */
	key_dir.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	key_dir.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	key_dir = Vector2Rotate(key_dir, camera_yaw);
	key_dir = Vector2Scale(key_dir, delta_time*CAM_SPEED*camera_scale);

	translate = (Vector3){key_dir.x, 0.0f, key_dir.y};

	/* ROTATION */
	if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
		mouse_dir = Vector2Scale(GetMouseDelta(), -ORBIT_SPEED);
		if(IsKeyDown(KEY_LEFT_SHIFT)) {
			translate.y = mouse_dir.y;
			mouse_dir.y = 0.0f;
			mouse_dir.x = 0.0f;
		}
	}
	
	offset = Vector3RotateByAxisAngle(offset, UP, mouse_dir.x);
	offset = Vector3RotateByAxisAngle(offset, basis_x, -mouse_dir.y);

	/* SCALING */
	if(wheel_mag) {
		if(camera_scale < 0.1f)
			wheel_mag = Clamp(wheel_mag, -999.9f, 0); //no more zoom in
		if(camera_scale > 40.0f)
			wheel_mag = Clamp(wheel_mag, 0, 999.9f); // assumption you can't scroll wheel faster
		
		offset = Vector3Scale(offset, 1 - wheel_mag);
	}

	/* APPLY TRANSFORMS */
	camera->target = Vector3Add(camera->target, translate);
	camera->position = Vector3Add(camera->target, offset);

	return;
}

#endif /* MD_CAMERA_H */
