#pragma once
#include <iostream>
#include <Eigen/Dense>
#include "../Object.h"
#include "../SolarSystem.h"
#include "../utils/utils_planetary.h"

class SolarSystem;

struct Kinematic {
	Object *anchor;
	Eigen::Vector3d p;
	Eigen::Vector3d v;
	Eigen::Vector3d a;
	SolarSystem* system;
};


class Kinematics {
public:
	Eigen::Vector3d p;
	Eigen::Vector3d v;
	Eigen::Vector3d a;

	Kinematics(SolarSystem* sys, Object* o);

	int initKinematicProcess(Eigen::Vector3d position, Eigen::Vector3d velocity);

	void setKinematicAnchor(Object* object) { anchor = object; }
	Object* getKinematicAnchor() { if (anchor) return anchor; else return NULL; }

	long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

private:
	bool initialized = false;
	Object* obj;
	Object* anchor;
	SolarSystem* system;
	void kinematicProcess();
};