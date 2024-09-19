/* =-------------------=
 * |	Duck Objects   |
 * |	Max Kawula     |
 * =-------------------=
 *
 * i just want to say, the choice to write moduloids 
 * arrived when i figured out that module is a keyword
 * that is all
 */

#ifndef MD_OBJECT_H
#define MD_OBJECT_H

#include <stddef.h>
#include "raylib.h"
#include "raymath.h"

#include "md_moduloid.h" // for function names

#define MAX_MODULES 4
#define MAX_PARAMETERS 4

typedef struct Duck_Object {
	Model model; // this contains original mesh
	short moduloids[MAX_MODULES];
	void* parameters[MAX_MODULES][MAX_PARAMETERS]; // i think currently there is no type safety for a wrong parameter type
} Duck_Object;

Duck_Object Create_Duck_Object(void)
{
	Duck_Object obj = { 0 };
	for(int i = 0; i < MAX_MODULES; ++i) {
		obj.moduloids[i] = -1;
	}
	return obj;
}

void Add_Moduloid(Duck_Object* obj, int moduloid)
{
	/* VALIDATION */
	if(obj == NULL) {
		TraceLog(LOG_ERROR, "No Duck Object Provided, did not add moduloid.");
		return;
	}

	/* ADD MODULOID */
	for(int i = 0; i < MAX_MODULES; ++i) {
		if(obj->moduloids[i] == -1) {
			obj->moduloids[i] = moduloid;
			TraceLog(LOG_INFO, "Successfully added moduloid:%d to slot[%d].", moduloid, i);
			return;
		}
	}

	TraceLog(LOG_WARNING, "No free slots, moduloid not added.");
	return;
}

void Apply_Moduloid_Stack(Duck_Object* obj)
{
	Matrix* base = &obj->model.transform;
	
	float a0 = 0.0f;
	float a1 = 0.0f;
	float a2 = 0.0f;
	// float a4 = 0.0f;

	// Vector3 b1;
	// Vector3 b2;
	// Vector3 b3;
	// Vector3 b4;
	
	// Matrix c0;
	
	for(int slot = 0; slot < MAX_MODULES; ++slot) {
		int id = obj->moduloids[slot];

		if(id >= NUM_MODULOIDS) {
			TraceLog(LOG_ERROR, "Invalid Moduloid - Slot:%d ID:%d", slot, id);
			return;
		}

		switch(id) {

		case MODULOID_SQUASH:
			a0 = *(float*)obj->parameters[slot][0];
			a1 = *(float*)obj->parameters[slot][1];
			*base = MatrixMultiply(*base, Moduloid_Squash(a0, a1));
			break;
		
		case MODULOID_HOVER:
			a0 = *(float*)obj->parameters[slot][0];
			a1 = *(float*)obj->parameters[slot][1];
			a2 = *(float*)obj->parameters[slot][2];
			*base = MatrixMultiply(*base, Moduloid_Hover(a0, a1, a2));
			break;
		case -1:
			break;
		}
	}
	return;
}

#endif /* MD_OBJECT_H */
