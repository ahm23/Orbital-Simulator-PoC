#pragma once

#include "Object.h"
#include "mechanics/Kinematics.h"

class Kinematics;

struct Element {
	Object* obj;
	Kinematics* kinematic;
};