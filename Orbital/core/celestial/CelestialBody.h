#pragma once
#include "../Object.h"

class CelestialBody : public Object {
public:
	CelestialBody(ObjectConfig config);
protected:
	double mu;
};
