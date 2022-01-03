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
	float ecliptic_i = 0;
};

struct PlanetConfigPackage {
	ObjectConfig config_obj;
	CelestialConfig config_clst;
	PlanetConfig config_planet;
};

class Planet: public CelestialBody {
public:
	Planet(PlanetConfigPackage config);
	~Planet();
private:
	PlanetConfig config_planet;
};