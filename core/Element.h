#pragma once
#include "Object.h"
#include "celestial/CelestialBody.h"

class Kinematics;

struct Element {
	Object* obj; //remove
	Object* anchor;
	std::vector<Object*> perturbers;
	std::unordered_map<int, CelestialBody*> depth_map;
	std::unordered_map<CelestialBody*, int> depth_map_reverse;
};