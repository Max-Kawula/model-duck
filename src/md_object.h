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
#define MAX_PARAMETERS 4

typedef struct Duck_Object {
	Model model; // this contains original mesh
	short moduloids[MAX_MODULOIDS];
	void* parameters[MAX_MODULOIDS][MAX_PARAMETERS]; // i think currently there is no type safety for a wrong parameter type
} Duck_Object;

Duck_Object Create_Duck_Object(void)
{
	Duck_Object obj = { 0 };
	for(int i = 0; i < MAX_MODULOIDS; ++i) {
		obj.moduloids[i] = -1;
	}
	return obj;
}

// when you add a moduloid, you NEED to add parameters
// otherwise you will crash!
int Add_Moduloid(Duck_Object* obj, int moduloid)
{
    	/* VALIDATION */
	if(obj == NULL) {
		TraceLog(LOG_ERROR, "No Duck Object Provided, did not add moduloid.");
		return -1;
	}

	/* ADD MODULOID */
	for(int i = 0; i < MAX_MODULOIDS; ++i) {
		if(obj->moduloids[i] == -1) {
			obj->moduloids[i] = moduloid;
			TraceLog(LOG_INFO, "Successfully added moduloid:%d to slot[%d].", moduloid, i);
			return i;
		}
	}

	TraceLog(LOG_WARNING, "No free slots, moduloid not added.");
	return -1;
}

void Remove_Moduloid(Duck_Object* obj, int slot)
{
	if(obj == NULL) {
		TraceLog(LOG_ERROR, "No Duck Object Provided.");
		return;
	}

	if(obj->moduloids[slot] == -1) {
		TraceLog(LOG_WARNING, "Tried to remove empty moduloid");
		return;
	}

	obj->moduloids[slot] = -1;
	for(int i = 0; i < MAX_PARAMETERS; ++i) {
		obj->parameters[slot][i] = NULL;
	}
}

void Apply_Moduloid(Duck_Object* obj, int slot)
{
	if(obj == NULL) {
		TraceLog(LOG_ERROR, "No Duck Object Provided.");
		return;
	}
	
	Matrix* base = &obj->model.transform;
	
	if(slot > MAX_MODULOIDS) {
		TraceLog(LOG_ERROR, "Moduloid:%d exceeds MAX_MODULOIDS. Not applying Moduloid.");
		return;
	}

	int id = obj->moduloids[slot];
	
	// i need to declare parameters before passing them
	// that or i just dereference when passing?
	float a0 = 0.0f;
	float a1 = 0.0f;
	float a2 = 0.0f;
	// float a4 = 0.0f;

	Vector3 b1;
	// Vector3 b2;
	// Vector3 b3;
	// Vector3 b4;
	
	// Matrix c0;
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
	case MODULOID_COPY_LOCATION:
		b1 = *(Vector3*)obj->parameters[slot][0];
		*base = MatrixMultiply(*base, Moduloid_Copy_Location(b1));
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
