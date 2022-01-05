#pragma once
#include <vector>

#include "Object.h"
#include "celestial/Star.h"
#include "celestial/Planet.h"
#include "Sattelite.h"

#include "utils/utils_parse.h"



class SolarSystem {
public:
	std::vector<std::string> starMap;
	std::vector<std::string> planetMap;

	std::vector<Star*> starList;
	std::vector<Planet*> planetList;

	SolarSystem();

	Object* getObjectFromName(ObjectTypes type, std::string name);

private:
	std::vector<Star*> loadStars();
	std::vector<Planet*> loadPlanets();
	std::vector<Sattelite*> loadSattelites();

	//void initializePlanetaryOrbit(int num);
	void initializeOrbit(int num, ObjectTypes type);

	//void initializeOrbit(OrbitInit init, Orbit* orbit, Eigen::Vector3d* Position, Eigen::Vector3d* Velocity);
};

