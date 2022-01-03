#pragma once
#include "../constants.h"
#include "CelestialBody.h"

struct StarConfig {
	double temperature;
};

struct StarConfigPackage {
	ObjectConfig config_obj;
	CelestialConfig config_clst;
	StarConfig config_star;
};

class Star : public CelestialBody {
public:
	Star(StarConfigPackage config);
private:
	StarConfig config;
	double luminosity;
};