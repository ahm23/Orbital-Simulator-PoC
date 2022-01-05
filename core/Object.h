#pragma once
#include <iostream>

#include <math.h>
#include <Eigen/Dense>

#include <thread>
#include <chrono>

#include "mechanics/Orbit.h"
#include "mechanics/Kinematics.h"

enum ObjectTypes {
	STAR,
	PLANET,
	MOON,
	ASTEROID,
	COMET,
	SATTELITE
};

struct ObjectConfig {
	std::string name;
	double mass;
	double mu;
};

class Object {
public:
	std::condition_variable kinematic_cv;
	std::mutex kinematic_m;

	Orbit orbit;
	Kinematic kinematics;
	
	Object(ObjectConfig config);
	~Object();

	std::string getName() { return config_obj.name; }
	double getMass() { return config_obj.mass; }

	void setMu(double mu) {
		//tmp function
		config_obj.mu = mu;
	}
	void setKinematicAnchor(Object* anchor) { kinematics.anchor = anchor; }

	void initKinematicProcess(Eigen::Vector3d position, Eigen::Vector3d velocity);
	unsigned long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

protected:
	static long update_freq;
	bool toggle_kinematic = false;

	ObjectConfig config_obj;

	void kinematicProcess();
};

