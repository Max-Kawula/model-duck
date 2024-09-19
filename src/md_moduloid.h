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

enum Moduloids {
	MODULOID_SQUASH,
	MODULOID_HOVER,
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

#endif /* MD_MODULOID_H */
