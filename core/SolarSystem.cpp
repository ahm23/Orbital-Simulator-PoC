#include "SolarSystem.h"

SolarSystem::SolarSystem()
{
	starList = loadStars();
    planetList = loadPlanets();

    for (int i = 0; i < planetList.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeOrbit(i + 1, PLANET);
    }
}

std::vector<Star*> SolarSystem::loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    std::vector<Star*> starList;
    parser.parseObjects<Star>(&starList);
    for (int i = 0; i < starList.size(); i++)
        starMap.push_back(starList[i]->getName());
    return starList;
}

std::vector<Planet*> SolarSystem::loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    std::vector<Planet*> planetList;
    //Planet** planetList = (Planet**)malloc(sizeof(Planet) * parser.getCount());
    parser.parseObjects<Planet>(&planetList);
    for (int i = 0; i < planetList.size(); i++)
        planetMap.push_back(planetList[i]->getName());
    return planetList;
}

std::vector<Sattelite*> SolarSystem::loadSattelites() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Sattelites.dat");
    std::vector<Sattelite*> satteliteList;
    parser.parseObjects<Sattelite>(&satteliteList);
    return satteliteList;
}

void SolarSystem::initializeOrbit(int num, ObjectTypes type) {
    Object* obj;
    std::string ref_object;
    Eigen::Vector3d Position, Velocity;
    int ref_type;
    
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\" + (std::string)filenames[type - 1]);

    switch (type) {
    case PLANET:
        obj = planetList[num - 1];
        break;
    default:
        return;
    }
    OrbitInit init = parser.parseOrbit(num, obj, &ref_type, &ref_object);
    init.init_mu = calculate_mu(obj->getMass(), starList[0]->getMass());

    obj->setKinematicAnchor(getObjectFromName((ObjectTypes)ref_type, ref_object));
    obj->setMu(init.init_mu);

    switch (init.type) {
    case 0:
        obj->orbit.initOrbitCOE_TA(init, &Position, &Velocity);
        break;
    case 1:
        obj->orbit.initOrbitCOE_R(init, &Position, &Velocity);
        break;
    case 2:
        obj->orbit.initOrbitTLE(init, &Position, &Velocity); //TODO
        break;
    case 3:
        obj->orbit.initOrbitCOE_ML(init, &Position, &Velocity);
        break;
    }
    obj->initKinematicProcess(Position, Velocity);
}

Object* SolarSystem::getObjectFromName(ObjectTypes type, std::string name) {
    switch (type) {
    case STAR:
        return starList[std::distance(starMap.begin(), std::find(starMap.begin(), starMap.end(), name))];
    case PLANET:
        return planetList[(int)(std::distance(planetMap.begin(), std::find(planetMap.begin(), planetMap.end(), name)))];
    default:
        return nullptr;
    }
}
