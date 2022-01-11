#pragma once

#include "Object.h"

class Kinematics;

struct Element {
	Object* obj; //remove
	Object* anchor;
	std::vector<Object*> perturbers;
	std::unordered_map<int, Object*> depth_map;
	std::unordered_map<Object*, int> depth_map_reverse;
};