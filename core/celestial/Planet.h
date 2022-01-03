#pragma once

#include <chrono>
#include <math.h>
#include <Eigen/Dense>
#include "../mechanics/Orbit.h"
#include "../mechanics/Kinetics.h"
#include "CelestialBody.h"
#include <condition_variable>

using namespace std;
using namespace std::chrono;
using namespace Eigen;

struct PlanetConfig {
};

struct PlanetConfigPackage {
	ObjectConfig config_obj;
	CelestialConfig config_clst;
	PlanetConfig config_planet;
};

class Planet: public CelestialBody {
public:

	std::condition_variable kinetic_cv;
	std::mutex kinetic_m;

	Kinetic kinetics;
	Orbit orbit;
	float ecliptic_i;
	double mu;

	Planet(PlanetConfigPackage config) : CelestialBody{ config.config_obj, config.config_clst } {
		//name = (char*)malloc(sizeof(char) * n.length());
		//mass = m;
		this->mu = mu;
		ecliptic_i = 0;
		orbit.mu = mu;
		kinetics.p << 0, 0, 0;
		kinetics.v << 0, 0, 0;
		kinetics.a << 0, 0, 0;
	}

	~Planet() {
		std::cout << "Destruct." << endl;
	}

	void initKineticProcess(Vector3d p, Vector3d v) {
		kinetics.p = p;
		kinetics.v = v;
		std::thread kineticThread([this] { this->kineticProcess(); });
		kineticThread.detach();
	}

	unsigned long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	const int update_freq = 100;


private:
	bool toggle_kinetic = true;

	void kineticProcess() {
		std::unique_lock<std::mutex> lk(kinetic_m);
		kinetic_cv.wait(lk, [&] {return toggle_kinetic; });
		while (true) {
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			//system("cls");
			kinetics.a = (-mu / pow(kinetics.p.norm(),3)) * kinetics.p;
			kinetics.v = kinetics.v + kinetics.a * (double)(time + update_freq + 1) / (double)1000 - kinetics.a * (double)time / (double)1000;
			kinetics.p = kinetics.p + kinetics.v * (double)(time + update_freq + 1) / (double)1000 - kinetics.v * (double)time / (double)1000;
			//std::cout << setprecision(12) << kinetics.p.norm() << endl;
			this_thread::sleep_for(chrono::milliseconds(1));
		}
	}

};