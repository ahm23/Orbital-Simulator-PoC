#pragma once
#include <math.h>

double kepler_solve(double init, double e) {
	double precision = 0.00001;
	double E = init;
	while (abs((E - e*sin(E)) - init) >= 0.00001) {
		E = E - (E - e * sin(E) - init) / (1 - e * cos(E));
	}
	return E;
}

