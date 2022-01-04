#pragma once
#include <math.h>
#include <iostream>
#include <iomanip>
#include <condition_variable>
#include <Eigen/Dense>
#include <Windows.h>

using namespace Eigen;

struct COE {
	double a = NULL;
	double e = NULL;
	double i = NULL;
	double raan = NULL;
	double omega = NULL;
	double TA = NULL;
};

struct OrbitInit {
	COE coe;
	unsigned int type;
	double init_r = NULL;
	double init_mu;
};

class Orbit {

public:
	COE coe;

	void initOrbitCOE_TA(OrbitInit init, Vector3d *p, Vector3d* v) {
		double init_r = (init.coe.a * (1 - pow(init.coe.e, 2))) / (1 + init.coe.e * cos(init.coe.TA));
		initOrbitCOE(init, p, v);
	}

	void initOrbitCOE_R(OrbitInit init, Vector3d* p, Vector3d* v) {
		init.coe.TA = -acos((init.coe.a * (1 - pow(init.coe.e, 2)) - init.init_r) / (init.init_r * init.coe.e));
		initOrbitCOE(init, p, v);
	}

	bool getInitStatus() {
		return init;
	}

private:
	bool init = false;

	void initOrbitCOE(OrbitInit init, Vector3d* p, Vector3d* v) {
		coe = init.coe;

		Matrix3d ROTATION{
			{cos(coe.raan) * cos(coe.omega) - sin(coe.raan) * cos(coe.i) * sin(coe.omega), -cos(coe.raan) * sin(coe.omega) - sin(coe.raan) * cos(coe.i) * cos(coe.omega), sin(coe.raan) * sin(coe.i)},
			{sin(coe.raan) * cos(coe.omega) + cos(coe.raan) * cos(coe.i) * sin(coe.omega), -sin(coe.raan) * sin(coe.omega) + cos(coe.raan) * cos(coe.i) * cos(coe.omega), -cos(coe.raan) * sin(coe.i)},
			{sin(coe.i) * sin(coe.omega), sin(coe.i) * cos(coe.omega), cos(coe.i)}
		};
		Vector3d position_p;
		Vector3d velocity_p;

		double r_dot = sqrt(init.init_mu / (coe.a * (1 - pow(coe.e, 2)))) * coe.e * sin(coe.TA);
		//double ta_dot = sqrt(init.init_mu / pow(a, 3)) * pow((1 + e * cos(init_ta)), 2) / pow(1 - pow(e, 2), 3 / 2);
		double rta_dot = sqrt(init.init_mu / (coe.a * (1 - pow(coe.e, 2)))) * (1+ coe.e * cos(coe.TA));


		position_p << init.init_r * (double)cos(coe.TA), init.init_r * (double)sin(coe.TA), 0;
		velocity_p << -sqrt(init.init_mu/ (coe.a * (1 - pow(coe.e, 2))))*sin(coe.TA), sqrt(init.init_mu / (coe.a * (1 - pow(coe.e, 2))))*(coe.e+cos(coe.TA)), 0;

		std::cout << position_p << std::endl;

		*p = ROTATION * position_p;
		*v = ROTATION * velocity_p;

		//std::cout << r_dot << std::endl << rta_dot;
		/*
		std::lock_guard<std::mutex> lk(kinetic_m);
		init = true;
		kinetic_cv.notify_one();*/
	}
};
