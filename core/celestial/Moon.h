#pragma once
#include <chrono>
#include <math.h>
#include <Eigen/Dense>
#include "../mechanics/Orbit.h"
#include "../mechanics/Kinematics.h"
#include "CelestialBody.h"
#include <condition_variable>

using namespace std;
using namespace std::chrono;
using namespace Eigen;

struct MoonConfig {
	float ecliptic_i = 0;
};

struct MoonConfigPackage {
	ObjectConfig config_obj;
	CelestialConfig config_clst;
	MoonConfig config_moon;
};

class Moon: public CelestialBody {
public:
	Moon(MoonConfigPackage config);
	~Moon();
private:
	MoonConfig config_moon;
};