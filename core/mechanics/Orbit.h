#pragma once
#include <math.h>
#include <iostream>
#include <iomanip>
#include <condition_variable>
#include <Eigen/Dense>
#include <Windows.h>

using namespace Eigen;

class Orbit {

public:
	//string name;
	double mu;
	double a;
	double e;
	double i;
	double raan;
	double omega;

	void initOrbitCOE_TA(double init_ta, double init_a, double init_e, double init_i, double init_raan, double init_omega, Vector3d *p, Vector3d* v) {
		double init_r = (a * (1 - pow(init_e, 2))) / (1 + init_e * cos(init_ta));
		initOrbitCOE(init_r, init_ta, init_a, init_e, init_i, init_raan, init_omega, p, v);
	}

	void initOrbitCOE_R(double init_r, double init_a, double init_e, double init_i, double init_raan, double init_omega, Vector3d* p, Vector3d* v) {
		double init_ta = -acos((init_a * (1 - pow(init_e, 2)) - init_r) / (init_r * init_e));
		initOrbitCOE(init_r, init_ta, init_a, init_e, init_i, init_raan, init_omega, p, v);
	}

	bool getInitStatus() {
		return init;
	}

private:
	bool init = false;

	void initOrbitCOE(double init_r, double init_ta, double init_a, double init_e, double init_i, double init_raan, double init_omega, Vector3d* p, Vector3d* v) {
		a = init_a;
		e = init_e;
		i = init_i;
		raan = init_raan;
		omega = init_omega;

		Matrix3d ROTATION{
			{cos(init_raan) * cos(init_omega) - sin(init_raan) * cos(init_i) * sin(init_omega), -cos(init_raan) * sin(init_omega) - sin(init_raan) * cos(init_i) * cos(init_omega), sin(init_raan) * sin(init_i)},
			{sin(init_raan) * cos(init_omega) + cos(init_raan) * cos(init_i) * sin(init_omega), -sin(init_raan) * sin(init_omega) + cos(init_raan) * cos(init_i) * cos(init_omega), -cos(init_raan) * sin(init_i)},
			{sin(init_i) * sin(init_omega), sin(init_i) * cos(init_omega), cos(init_i)}
		};
		Vector3d position_p;
		Vector3d velocity_p;

		double r_dot = sqrt(mu / (a * (1 - pow(e, 2)))) * e * sin(init_ta);
		//double ta_dot = sqrt(mu / pow(a, 3)) * pow((1 + e * cos(init_ta)), 2) / pow(1 - pow(e, 2), 3 / 2);
		double rta_dot = sqrt(mu / (a * (1 - pow(e, 2)))) * (1+ e * cos(init_ta));


		position_p << init_r * (double)cos(init_ta), init_r * (double)sin(init_ta), 0;
		velocity_p << -sqrt(mu/ (a * (1 - pow(e, 2))))*sin(init_ta), sqrt(mu / (a * (1 - pow(e, 2))))*(e+cos(init_ta)), 0;

		*p = ROTATION * position_p;
		*v = ROTATION * velocity_p;

		std::cout << r_dot << std::endl << rta_dot;
		/*
		std::lock_guard<std::mutex> lk(kinetic_m);
		init = true;
		kinetic_cv.notify_one();*/
	}

};

