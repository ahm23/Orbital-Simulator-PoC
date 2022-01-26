#pragma once
#include "../Object.h"

struct CelestialConfig {
	double avg_radius;
	float ecliptic_i = 0;
	//TODO: composition
};

class CelestialBody;

class CelestialBody : public Object {
public:
	CelestialBody(ObjectConfig config_obj, CelestialConfig config_clst);
	float getEcliptic() { return config_clst.ecliptic_i; }
protected:
	CelestialConfig config_clst;
};
