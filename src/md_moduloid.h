/*	md_moduloid.h
 *	max kawula 2024
 * 
 * this is a helper tool for running raymath.h functions
 * on different structures of vertices.
 *
 * seems like I'm not using it, but maybe later.
 */

#ifndef MD_MODULOID_H
#define MD_MODULOID_H

#include "raylib.h"
#include "raymath.h"
#include "md_entities.h"

enum Moduloids {
	MODULOID_SQUASH,
	MODULOID_HOVER,
	MODULOID_COPY_LOCATION,
	MODULOID_CONTROLLER_TO_QUATERNION,
	MODULOID_WOBBLE,
	MODULOID_MOVE,
	NUM_MODULOIDS
};

// freq is oscillations per second, default 1 squish/second
// amplitude is the percentage squish, clamped to 1.00f
Matrix Moduloid_Squash(float frequency, float amplitude)
{
	float offset = GetTime() * frequency*2.0f*PI;
	Matrix result = MatrixScale(1.0f-sinf(offset)*amplitude, 1.0f-cosf(offset)*amplitude, 1.0f-sinf(offset)*amplitude);

	return result;
}

Matrix Moduloid_Hover(float frequency, float amplitude, float offset)
{
	float wave = sinf(GetTime()*frequency*2.0f*PI)*amplitude + offset;
	Matrix result = MatrixTranslate(0.0f, wave, 0.0f);

	return result;
}

Matrix Moduloid_Copy_Location(Vector3 target)
{
	Matrix result = MatrixTranslate(target.x, target.y, target.z);
	return result;
}

Matrix Moduloid_Controller_To_Quaternion(void)
{
	Matrix result = MatrixIdentity();
	Quaternion rotation = QuaternionIdentity();

	rotation.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
	rotation.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
	rotation.z = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

	rotation.w = 0.5f*(1.0f + GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER));

	result = QuaternionToMatrix(rotation);
	return result;
}

// speed is a factor between 0..1
Matrix Moduloid_Wobble(float speed)
{
	Matrix result = MatrixIdentity();
	const float factor = 25.0f;
	float time = Wrap(GetTime(), -PI, PI);
	Matrix y_wobble = MatrixRotateY(speed*0.5f*sinf(factor*time));
	Matrix z_wobble = MatrixRotateZ(-speed*0.5f*cosf(factor*time));
	Matrix x_wobble = MatrixRotateX(speed*PI/6.0f);
	result = MatrixMultiply(y_wobble, z_wobble);
	result = MatrixMultiply(result, x_wobble);
	return result;
}

Matrix Moduloid_Move(Entity* entity)
{
	Matrix result = MatrixIdentity();
	float speed = Vector3Length(entity->vel)/MAX_SPEED;

	result = Moduloid_Wobble(speed);
	
	result = MatrixMultiply(result, Moduloid_Squash(1.0f, 0.05f));
	result = MatrixMultiply(result, MatrixRotateY(entity->yaw));
	result = MatrixMultiply(result, Moduloid_Copy_Location(entity->pos));

	return result;
}
#endif /* MD_MODULOID_H */
