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
	MODULOID_WOBBLE,
	MODULOID_MOVE,
	MODULOID_SPLAT,
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

Matrix Moduloid_Wobble(float speed)
{
	Matrix result = MatrixIdentity();
	float factor = 17.5f; // affects frequency!
	float time = Wrap(GetTime(), -PI, PI);

	float y_bob = speed*0.5f + speed*0.2f*cosf(2*factor*time);
	
	float yaw = speed*0.5f*sinf(factor*time);
	float roll = -speed*0.5f*cosf(factor*time);
	float pitch = speed*PI/6.0f;
	
	Quaternion rotation = QuaternionFromEuler(pitch, yaw, roll);
	result = QuaternionToMatrix(rotation);
	result.m13 = y_bob; //translation

	return result;
}

Matrix Moduloid_Move(Entity* entity)
{
	Matrix result = MatrixIdentity();
	float speed = Vector3Length(entity->vel)/MAX_SPEED; 
	speed = Clamp(speed, 0.0f, 1.0f); // speed can exceed MAX_SPEED, could result in wonky rotations

	result = Moduloid_Squash(1.0f, 0.05f);
	
	result = MatrixMultiply(result, Moduloid_Wobble(speed));
	result = MatrixMultiply(result, MatrixRotateY(entity->yaw));
	result = MatrixMultiply(result, Moduloid_Copy_Location(entity->pos));

	return result;
}

Matrix Moduloid_Splat(float intensity, float* time_elapsed)
{
	Matrix result = MatrixIdentity();

	if(*time_elapsed == -1.0f) {
		return result;
	}

	*time_elapsed += GetFrameTime();

	const float duration = 2.0f;

	if(*time_elapsed > duration) {
		*time_elapsed = -1.0f;
		return result;
	}

	float animation_param = 1.0f + *time_elapsed/-duration; // ranges from 0..1

	float splat = 1.0f + animation_param*animation_param * -sinf((*time_elapsed)*intensity);

	result = MatrixScale(2.0f-splat, splat, 2.0f-splat);

	return result;
}

#endif /* MD_MODULOID_H */
