#pragma once
#include <math.h>
#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class Orbit {
	double a;
	float e;
	float i;
	float raan;
	float omega;
public:
	//string name;

	void initOrbitCOE_TA(float init_ta, double init_a, float init_e, float init_i, float init_raan, float init_omega) {
		double init_r = (a * (1 - pow(init_e, 2))) / (1 + init_e * cos(init_ta));
		initOrbitCOE(init_r, init_ta, init_a, init_e, init_i, init_raan, init_omega);
	}

	void initOrbitCOE_R(double init_r, double init_a, float init_e, float init_i, float init_raan, float init_omega) {
		float init_ta = acos((init_a * (1 - pow(init_e, 2)) - init_r) / (init_r * init_e));
		initOrbitCOE(init_r, init_ta, init_a, init_e, init_i, init_raan, init_omega);
	}

private:
	void initOrbitCOE(double init_r, float init_ta, double init_a, float init_e, float init_i, float init_raan, float init_omega) {
		std::cout << init_ta << endl;
		Matrix3d ROTATION{
			{cos(init_raan) * cos(init_omega) - sin(init_raan) * cos(init_i) * sin(init_omega), -cos(init_raan) * sin(init_omega) - sin(init_raan) * cos(init_i) * cos(init_omega), sin(init_raan) * sin(init_i)},
			{sin(init_raan) * cos(init_omega) + cos(init_raan) * cos(init_i) * sin(init_omega), -sin(init_raan) * sin(init_omega) + cos(init_raan) * cos(init_i) * cos(init_omega), -cos(init_raan) * sin(init_i)},
			{sin(init_i) * sin(init_omega), sin(init_i) * cos(init_omega), cos(init_i)}
		};
		Vector3d Perifocal;
		Vector3d Inertial;

		Perifocal << init_r * cos(init_ta), init_r* sin(init_ta), 0;
		Inertial = ROTATION * Perifocal;
		std::cout << Inertial << endl;
	}
};

