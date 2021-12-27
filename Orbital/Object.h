#pragma once
#include "Orbit.h"
#include "Kinetics.h"
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Object {
public:
	double mass;
	//float* shape;
	Kinetic kinetics;
	//Orbit orbit;
	unsigned long time =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	const int iv = 100;
	const int mu = 398600;

	Object(double m, double pos[3], double vel[3]) {
		int i;
		mass = m;
		/*kinetics.p = (double*)malloc(sizeof(double) * 3);
		kinetics.v = (double *)malloc(sizeof(double) * 3);
		kinetics.a = (double *)malloc(sizeof(double) * 3);
		if (kinetics.p == NULL || kinetics.v == NULL || kinetics.a == NULL) {
			fprintf(stderr, "Fatal: failed to allocate memory for initial kinematics.\n");
			abort();
		}
		for (i = 0; i < 3; i++)
			kinetics.p[i] = pos[i];
		for (i = 0; i < 3; i++)
			kinetics.v[i] = vel[i];
		for (i = 0; i < 3; i++)
			kinetics.a[i] = -mu * pos[i] / pow(sqrt(pow(pos[0], 2) + pow(pos[1], 2) + pow(pos[2], 2)), 3);
		init();*/
	}

	double* getPosition() {
		//return kinetics.p;
	}


private:

	void init() {
		std::thread kineticThread([this] { this->kineticProcess(); });
		kineticThread.detach();
	}

	void kineticProcess() {
		int i;
		while (true) {
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			system("cls");
			//cout << (float)time / (float)1000;
			//cout << '\n';
			//cout << (float)(time + iv) / (float)1000;
			for (i = 0; i < 3; i++) {
				cout << kinetics.p[i];
				cout << '\n';
				kinetics.a[i] = -mu * kinetics.p[i] / pow(sqrt(pow(kinetics.p[0], 2) + pow(kinetics.p[1], 2) + pow(kinetics.p[2], 2)), 3);
				kinetics.v[i] = kinetics.v[i] + kinetics.a[i] * (double)(time + iv)/ (double)1000 - kinetics.a[i] * (double)time/ (double)1000;
				kinetics.p[i] = kinetics.p[i] + kinetics.v[i] * (double)(time + iv)/ (double)1000 - kinetics.v[i] * (double)time/ (double)1000;
			}
			this_thread::sleep_for(chrono::milliseconds(1));
		}
	}
};

