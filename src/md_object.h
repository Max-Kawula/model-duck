/* =-------------------=
 * |	Duck Objects   |
 * |	Max Kawula     |
 * =-------------------=
 *
 * Duck Objects are a way to keep track of moduloids and their parameters. 
 * Moduloids are simple in concept, they just produce a transform given parameters.
 * Duck Objects store references to a specific function, and also provide references for
 * parameters.
 *
 * What is nice about this system is that you don't need to use any functions to use 
 * the duck object. You could manually lookup.
 */

#ifndef MD_OBJECT_H
#define MD_OBJECT_H

#include <stddef.h>
#include "raylib.h"
#include "raymath.h"

#include "md_moduloid.h" // for function names

#define MAX_MODULOIDS 4
#define MAX_PARAMETERS 9 // num of 32-bit values (most things are floats anyways)

typedef struct Moduloid {
	short id;
	union Parameter {
		float value[MAX_PARAMETERS];
		int number[MAX_PARAMETERS];
		Vector3 vec3[MAX_PARAMETERS/3];
		Vector4 vec4[MAX_PARAMETERS/4];
		Quaternion quat[MAX_PARAMETERS/4];
		void* reference[MAX_PARAMETERS/2]; //assuming void* is 64 bits :)
	} param;
} Moduloid;


typedef struct Duck_Object {
	Model model;
	Moduloid moduloid[MAX_MODULOIDS];
} Duck_Object;

Duck_Object Create_Duck_Object(void)
{
	Duck_Object obj = { 0 };
	for(int i = 0; i < MAX_MODULOIDS; ++i) {
		obj.moduloid[i].id = -1;
	}
	return obj;
}

void Apply_Moduloid(Duck_Object* obj, int slot)
{
	Matrix* base = &obj->model.transform;
	
	if(slot > MAX_MODULOIDS) {
		TraceLog(LOG_ERROR, "Moduloid:%d exceeds MAX_MODULOIDS. Not applying Moduloid.");
		return;
	}

	int id = obj->moduloid[slot].id;
	
	if(id >= NUM_MODULOIDS) {
		TraceLog(LOG_ERROR, "Invalid Moduloid - Slot:%d ID:%d", slot, id);
		return;
	}

	union Parameter* p = &obj->moduloid[slot].param;

	switch(id) {
	case MODULOID_SQUASH:
		*base = MatrixMultiply(*base, Moduloid_Squash(p->value[0], p->value[1]));
		break;
	case MODULOID_HOVER:
		*base = MatrixMultiply(*base, Moduloid_Hover(p->value[0], p->value[1], p->value[2]));
		break;
	case MODULOID_COPY_LOCATION:
		*base = MatrixMultiply(*base, Moduloid_Copy_Location(p->vec3[0]));
		break;
	case MODULOID_WOBBLE:
		*base = MatrixMultiply(*base, Moduloid_Wobble(p->value[0]));
		break;
	case MODULOID_MOVE:
		*base = MatrixMultiply(*base, Moduloid_Move((Entity*)p->reference[0]));
		break;
	case MODULOID_SPLAT:
		*base = MatrixMultiply(*base, Moduloid_Splat(p->value[0], &p->value[1]));
		break;
	case -1:
		break;
	}
	return;
}

void Apply_Moduloid_Stack(Duck_Object* obj)
{
	for(int slot = 0; slot < MAX_MODULOIDS; ++slot) {
		Apply_Moduloid(obj, slot);
	}
	return;
}

#endif /* MD_OBJECT_H */
