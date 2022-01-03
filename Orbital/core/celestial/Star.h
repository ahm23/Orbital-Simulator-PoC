#pragma once
#include "../constants.h"
#include "CelestialBody.h"

struct StarConfig {
	double avg_radius;
	double temperature;
};

struct StarConfigPackage {
	ObjectConfig config_obj;
	StarConfig config_star;
};

class Star : public CelestialBody {
public:
	Star(StarConfigPackage config);
private:
	StarConfig config;
	double luminosity;
};