#pragma once

#include <chrono>
#include <math.h>
#include "Orbit.h"

using namespace std;
using namespace std::chrono;

class CelestialBody {
public:
	char* name;
	double mass;


private:

};

class Star : public CelestialBody {
public:
	Star(string n, double m) {
		name = (char*)malloc(sizeof(char) * n.length());
		
		mass = m;

	}
};

class Planet: public CelestialBody {
public:
	Orbit orbit;
	float ecliptic_i;
	double r_init;

	Planet(double m, float ei) {
		mass = m;
		ecliptic_i = ei;
	}




	unsigned long time =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	const int iv = 100;



private:

};