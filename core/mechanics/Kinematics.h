#pragma once
#include <iostream>
#include <Eigen/Dense>
#include "../Object.h"
#include "../SolarSystem.h"

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

	Kinematics(SolarSystem* sys);

	int initKinematicProcess(Eigen::Vector3d position, Eigen::Vector3d velocity);
	long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

private:
	bool initialized = false;
	Object* anchor = (Object*)nullptr;
	SolarSystem* system;
	void kinematicProcess();
};