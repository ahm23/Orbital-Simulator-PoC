#pragma once
#include <iostream>

#include <math.h>
#include <Eigen/Dense>

#include <thread>
#include <chrono>

#include "mechanics/Orbit.h"

enum ObjectTypes {
	BARYCENTRE = 0,
	STAR = 1,
	PLANET = 2,
	MOON = 3,
	ASTEROID = 4,
	COMET = 5,
	SATTELITE = 6
};

static const char* filenames[] = { "Stars.dat", "Planets.dat" };

struct ObjectConfig {
	std::string name = "N/A";
	double mass = 0;
	double mu = 0;
};

class Object {
public:
	std::condition_variable kinematic_cv;
	std::mutex kinematic_m;

	Orbit orbit;
	//Kinematic kinematics;
	
	Object(ObjectConfig config);
	~Object();

	std::string getName() { return config_obj.name; }
	double getMass() { return config_obj.mass; }

	void setMu(double mu) {
		//tmp function
		config_obj.mu = mu;
	}
	//void setKinematicAnchor(Object* anchor) { kinematics.anchor = anchor; }

	//void initKinematicProcess(Eigen::Vector3d position, Eigen::Vector3d velocity);
	long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

protected:

	ObjectConfig config_obj;

	//void kinematicProcess();
};

