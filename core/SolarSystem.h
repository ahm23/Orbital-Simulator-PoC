#pragma once
#include <vector>

#include "celestial/Star.h"
#include "celestial/Planet.h"
#include "Sattelite.h"

#include "utils/utils_parse.h"



class SolarSystem {
public:
	std::vector<Star*> starList;
	std::vector<Planet*> planetList;

	SolarSystem();

private:
	std::string* planetMap;
	std::string* starMap;

	std::vector<Star*> loadStars();
	std::vector<Planet*> loadPlanets();
	std::vector<Sattelite*> loadSattelites();

	void initializePlanetaryOrbit(int num);
	void initializeOrbit(OrbitInit init, Orbit* orbit, Eigen::Vector3d* Position, Eigen::Vector3d* Velocity);
};

