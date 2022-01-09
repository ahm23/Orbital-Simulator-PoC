#pragma once
#include <vector>

#include "Object.h"
#include "celestial/Star.h"
#include "celestial/Planet.h"
#include "Sattelite.h"

#include "Element.h"

#include "utils/utils_parse.h"

#include <shared_mutex>



class SolarSystem {
public:
	std::vector<std::string> starMap;
	std::vector<std::string> planetMap;

	std::vector<Element*> starList;
	std::vector<Element*> planetList;

	SolarSystem();

	Object* getObjectFromName(ObjectTypes type, std::string name);
	Element* getElementFromName(ObjectTypes type, std::string name);
	Eigen::Vector3d getVectorBetweenObjects(Element* o1, Element* o2);
	void mapSystem();

	static long update_freq;
	bool toggle_kinematic = false;
	std::condition_variable_any kinematic_cv;
	std::shared_mutex kinematic_m;

private:
	std::vector<Element*> loadStars();
	std::vector<Element*> loadPlanets();
	std::vector<Element*> loadSattelites();

	//void initializePlanetaryOrbit(int num);
	void initializeMechanics(int num, ObjectTypes type);
	void start();
	//void initializeOrbit(OrbitInit init, Orbit* orbit, Eigen::Vector3d* Position, Eigen::Vector3d* Velocity);
};

