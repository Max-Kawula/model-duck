#ifndef MD_ENTITITES_H
#define MD_ENTITITES_H

#include "raylib.h"
#include "raymath.h"

#define MAX_SPEED (8.00f)
#define MAX_ACCEL (4.00f)
#define FRICTION (5.00f) 
#define MIN_SPEED (0.0001f)

typedef struct {
	Vector3 pos;
	Vector3 vel;
	float yaw;
} Entity;

void Control_Entity(Entity* entity, Camera* camera)
{
	float delta_time = GetFrameTime();
	
	Vector3 view_offset = Vector3Subtract(camera->position, camera->target);
	float c_yaw = -atan2f(view_offset.x, view_offset.z);

	Vector2 move_dir = Vector2Zero();
	move_dir.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	move_dir.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

	move_dir = Vector2Rotate(move_dir, c_yaw);
	if(Vector2Length(move_dir) > 1.0f) {
		move_dir = Vector2Normalize(move_dir);
	}

	Vector2 vel_2D = {entity->vel.x, entity->vel.z};

	float accel_speed = 0.0f;
	float deaccel_speed = 0.0f;
	float target_speed = Vector2Length(move_dir) * MAX_SPEED;
	float current_speed = Vector2DotProduct(vel_2D, move_dir);
	float move_speed = Vector2Length(vel_2D);

	float add_speed = target_speed - current_speed;

	accel_speed = MAX_ACCEL * delta_time * target_speed;
	if(accel_speed > add_speed)
		accel_speed = add_speed;

	if(move_speed > MIN_SPEED) {

		deaccel_speed = move_speed - delta_time * move_speed * FRICTION;

		if(deaccel_speed < 0)
			deaccel_speed = 0;
		deaccel_speed /= move_speed;
	}

	if(target_speed) {
		float target_yaw = Vector2Angle(move_dir, (Vector2){0.0f, 1.0f}) - entity->yaw; // direction penger wants to rotate
		target_yaw = Wrap(target_yaw, -PI, PI);
		entity->yaw += delta_time*8.0f*target_yaw;
	} else {
		entity->vel = Vector3Zero();
		return;
	}

	Vector3 move_dir_3D = {move_dir.x, 0.0f, move_dir.y};

	entity->vel = Vector3Scale(entity->vel, deaccel_speed);
	entity->vel = Vector3Add(Vector3Scale(move_dir_3D, accel_speed), entity->vel);
	entity->pos = Vector3Add(Vector3Scale(entity->vel, delta_time), entity->pos);

	return;
}
	
#endif /* MD_ENTITITES_H */
