#include "lab_m1/Tema3/Structs3.h"

bool IsOnGround(Obj object, Ground ground)
{
	float x1 = ground.center.x - ground.scalation.x / 2.0f;
	float x2 = ground.center.x + ground.scalation.x / 2.0f;
	float z1 = ground.center.z - ground.scalation.x / 2.0f * cos(RADIANS(ground.rotation.x));
	float z2 = ground.center.z + ground.scalation.x / 2.0f * cos(RADIANS(ground.rotation.x));
	if (object.center.x > x1 && object.center.x < x2 && object.center.z > z1 && object.center.z < z2)
		return true;

	return false;
}