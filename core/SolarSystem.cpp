#include "SolarSystem.h"

SolarSystem::SolarSystem()
{
	starList = loadStars();
    planetList = loadPlanets();

    for (int i = 0; i < planetList.size(); i++) {
        initializePlanetaryOrbit(i + 1);
    }
}

std::vector<Star*> SolarSystem::loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    std::vector<Star*> starList;
    parser.parseObjects<Star>(&starList);
    starMap = new std::string[parser.getCount()];
    for (int i = 0; i < starList.size(); i++)
        starMap[i] = starList[i]->getName();
    return starList;
}

std::vector<Planet*> SolarSystem::loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    std::vector<Planet*> planetList;
    //Planet** planetList = (Planet**)malloc(sizeof(Planet) * parser.getCount());
    parser.parseObjects<Planet>(&planetList);
    planetMap = new std::string[parser.getCount()];
    for (int i = 0; i < planetList.size(); i++)
        planetMap[i] = planetList[i]->getName();
    return planetList;
}

std::vector<Sattelite*> SolarSystem::loadSattelites() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Sattelites.dat");
    std::vector<Sattelite*> satteliteList;
    parser.parseObjects<Sattelite>(&satteliteList);
    return satteliteList;
}

void SolarSystem::initializePlanetaryOrbit(int num) {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");

    Eigen::Vector3d Position, Velocity;
    std::string ref_object;
    Planet* planet = planetList[num - 1];

    OrbitInit init = parser.parseOrbit(num, planet, &ref_object);

    init.init_mu = calculate_mu(planet->getMass(), starList[0]->getMass());

    //planet->setKinematicAnchor(ref_object);
    planet->setMu(init.init_mu);

    initializeOrbit(init, &(planet->orbit), &Position, &Velocity);

    planet->initKinematicProcess(Position, Velocity);
}

void SolarSystem::initializeOrbit(OrbitInit init, Orbit* orbit, Eigen::Vector3d* Position, Eigen::Vector3d* Velocity) {
    switch (init.type) {
    case 0:
        orbit->initOrbitCOE_TA(init, Position, Velocity);
        break;
    case 1:
        orbit->initOrbitCOE_R(init, Position, Velocity);
        break;
    case 2:
        orbit->initOrbitTLE(init, Position, Velocity); // TODO
        break;
    case 3:
        orbit->initOrbitCOE_ML(init, Position, Velocity);
        break;
    }
}