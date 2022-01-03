#pragma once
#include <iostream>

#include <math.h>
#include <Eigen/Dense>

#include <thread>
#include <chrono>

#include "mechanics/Orbit.h"
#include "mechanics/Kinetics.h"

struct ObjectConfig {
	std::string name;
	double mass;
	double mu;
};

class Object {
public:
	std::condition_variable kinetic_cv;
	std::mutex kinetic_m;

	Orbit orbit;
	Kinetic kinetics;
	
	Object(ObjectConfig config);
	~Object();

	void initKineticProcess(Eigen::Vector3d position, Eigen::Vector3d velocity);
	unsigned long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

protected:
	static long update_freq;
	bool toggle_kinetic = false;

	ObjectConfig config;

	void kineticProcess();
};
