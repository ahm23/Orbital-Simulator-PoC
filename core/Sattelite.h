#pragma once

#include <chrono>
#include <math.h>
#include <Eigen/Dense>
#include "mechanics/Orbit.h"
#include "mechanics/Kinematics.h"
#include "Object.h"
#include <condition_variable>

using namespace std;
using namespace std::chrono;
using namespace Eigen;

struct SatteliteConfig {
	
};

struct SatteliteConfigPackage {
	ObjectConfig config_obj;
	SatteliteConfig config_sat;
};

class Sattelite: public Object {
public:
	Sattelite(SatteliteConfigPackage config);
	~Sattelite();
private:
	SatteliteConfig config_sat;
};