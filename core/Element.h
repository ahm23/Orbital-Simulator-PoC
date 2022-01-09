#pragma once

#include "Object.h"
#include "mechanics/Kinematics.h"

class Kinematics;

struct Element {
	Object* obj; //remove
	Kinematics* kinematic;
	std::unordered_map<int, Object*> depth_map;
	std::unordered_map<Object*, int> depth_map_reverse;
};

struct Element_Obj : Element {
	Object* obj;
};

struct Element_Clst : Element {
	CelestialBody* clst;
};