#include "SolarSystem.h"

long SolarSystem::update_freq = 1000;

SolarSystem::SolarSystem()
{
	starList = loadStars();
    planetList = loadPlanets();

    for (int i = 0; i < planetList.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeMechanics(i + 1, PLANET);
    }

    std::lock_guard<shared_mutex> lk(kinematic_m);
    toggle_kinematic = true;
    kinematic_cv.notify_all();
}

std::vector<Element*> SolarSystem::loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    std::vector<Element*> starList;
    parser.parseObjects<Element, Star>(&starList);
    for (int i = 0; i < starList.size(); i++)
        starMap.push_back(starList[i]->obj->getName());
    return starList;
}

std::vector<Element*> SolarSystem::loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    std::vector<Element*> planetList;
    parser.parseObjects<Element, Planet>(&planetList);
    for (int i = 0; i < planetList.size(); i++) {
        planetList[i]->kinematic = new Kinematics(this);
        planetMap.push_back(planetList[i]->obj->getName());
    }
    return planetList;
}

std::vector<Element*> SolarSystem::loadSattelites() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Sattelites.dat");
    std::vector<Element*> satteliteList;
    parser.parseObjects<Element, Sattelite>(&satteliteList);
    return satteliteList;
}

void SolarSystem::initializeMechanics(int num, ObjectTypes type) {
    Object* obj;
    std::string ref_object;
    Eigen::Vector3d Position, Velocity;
    int ref_type;
    
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\" + (std::string)filenames[type - 1]);

    switch (type) {
    case PLANET:
        obj = planetList[num - 1]->obj;
        break;
    default:
        return;
    }
    OrbitInit init = parser.parseOrbit(num, obj, &ref_type, &ref_object);
    init.init_mu = calculate_mu(obj->getMass(), starList[0]->obj->getMass());

    //obj->setKinematicAnchor(getObjectFromName((ObjectTypes)ref_type, ref_object));
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
    if (planetList[num - 1]->kinematic->initKinematicProcess(Position, Velocity)) {
        std::cout << "\033[0;32;49mSUCCESS: Initialized Kinematic Process for Object: " << obj->getName() << "\033[0m" << std::endl;
    }
    else {
        std::cout << "WARN: Attempt to Initialize Kinematic Process Failed for Object: " << obj->getName() << std::endl;
        std::cout << "Reason | Kinematic Process Already Initialized" << std::endl;
    }
}

Object* SolarSystem::getObjectFromName(ObjectTypes type, std::string name) {
    switch (type) {
    case STAR:
        return starList[std::distance(starMap.begin(), std::find(starMap.begin(), starMap.end(), name))]->obj;
    case PLANET:
        return planetList[(int)(std::distance(planetMap.begin(), std::find(planetMap.begin(), planetMap.end(), name)))]->obj;
    default:
        return nullptr;
    }
}
