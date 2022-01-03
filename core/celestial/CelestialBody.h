#pragma once
#include "../Object.h"

struct CelestialConfig {
	double avg_radius;
	//TODO: composition
};

class CelestialBody : public Object {
public:
	CelestialBody(ObjectConfig config_obj, CelestialConfig config_clst);
protected:
	CelestialConfig config;
	double mu = 0;
};
