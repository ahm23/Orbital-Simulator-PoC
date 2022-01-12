#include "SolarSystem.h"

long SolarSystem::update_freq = 1000;

SolarSystem::SolarSystem() {

	loadStars();
    loadPlanets();

    for (int i = 0; i < iBuffer_star.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeMechanics(i, iBuffer_star[i], STAR);
    }

    for (int i = 0; i < iBuffer_planet.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeMechanics(i, iBuffer_planet[i], PLANET);
    }

    mapSystem();
    ke = new KinematicEngine(&elements, 1, &kinematic_m, &kinematic_cv);

    std::lock_guard<std::shared_mutex> lk(kinematic_m);
    toggle_kinematic = true;
    kinematic_cv.notify_all();
}

std::vector<Element*> SolarSystem::loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    std::vector<Element*> starList;
    int start = (int)elements.size();
    int count = parser.parseObjects<Element, Star>(&elements);
    for (int i = start; i < count; i++) {
        iBuffer_star.push_back(i);
        starMap[elements[i]->obj->getName()] = elements[i]->obj->getID();
    }
    return starList;
}

std::vector<Element*> SolarSystem::loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    std::vector<Element*> planetList;
    int start = (int)elements.size();
    int count = parser.parseObjects<Element, Planet>(&elements);
    for (int i = start; i < start + count; i++) {
        iBuffer_planet.push_back(i);
        planetMap[elements[i]->obj->getName()] = elements[i]->obj->getID();
    }
    return planetList;
}

std::vector<Element*> SolarSystem::loadSattelites() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Sattelites.dat");
    std::vector<Element*> satteliteList;
    parser.parseObjects<Element, Sattelite>(&elements);
    return satteliteList;// INCOMPLETE
}

void SolarSystem::initializeMechanics(int index, int num, ObjectTypes type) {
    Element* el;
    std::string ref_object;
    Eigen::Vector3d Position, Velocity;
    int ref_type;
    
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\" + (std::string)filenames[type - 1]);

    switch (type) {
    case STAR:
        el = elements[num];
        break;
    case PLANET:
        el = elements[num];
        break;
    default:
        return;
    }
    OrbitInit init = parser.parseOrbit(index, el->obj, &ref_type, &ref_object);
    if (ref_type == ObjectTypes::BARYCENTRE) {
        init.init_mu = 0;
        el->anchor = NULL;
    }
    else {
        el->anchor = getObjectFromName((ObjectTypes)ref_type, ref_object);
        init.init_mu = calculate_mu(el->obj->getMass(), el->anchor->getMass());
    }

    el->obj->setMu(init.init_mu);
    switch (init.type) {
        case 0:
            el->obj->orbit.initOrbitCOE_TA(init, &Position, &Velocity);
            break;
        case 1:
            el->obj->orbit.initOrbitCOE_R(init, &Position, &Velocity);
            break;
        case 2:
            el->obj->orbit.initOrbitTLE(init, &Position, &Velocity); //TODO
            break;
        case 3:
            el->obj->orbit.initOrbitCOE_ML(init, &Position, &Velocity);
            break;
    }
    el->obj->setPos(Position);
    el->obj->setVel(Velocity);
    /*
    if (el->kinematic->initKinematicProcess(Position, Velocity)) {
        std::cout << "\033[0;32;49mSUCCESS: Initialized Kinematic Process for Object: " << el->obj->getName() << "\033[0m" << std::endl;
    }
    else {
        std::cout << "WARN: Attempt to Initialize Kinematic Process Failed for Object: " << el->obj->getName() << std::endl;
        std::cout << "Reason | Kinematic Process Already Initialized" << std::endl;
    }*/ 
}


void SolarSystem::mapSystem() {
    for (int i = 0; i < elements.size(); i++) {
        Element* el = elements[i];
        Element* el_tmp = el;
        int iterator = 0;
        CelestialBody* next_anchor;
        while (iterator > -1) {
            el->depth_map[iterator] = (CelestialBody*)el_tmp->obj;
            el->depth_map_reverse[(CelestialBody*)el_tmp->obj] = iterator;
            next_anchor = (CelestialBody*)el_tmp->anchor;      /// might revert to Objects & cast to clst in utils_planetary
            if (!next_anchor || next_anchor == NULL)
                break;
            el_tmp = getElementFromName(next_anchor->getType(), next_anchor->getName());
            iterator++;
        }
    }
}

void SolarSystem::start() {

}


Object* SolarSystem::getObjectFromName(ObjectTypes type, std::string name) {
    switch (type) {
    case STAR:
        return elements[starMap[name]]->obj;
    case PLANET:
        return elements[planetMap[name]]->obj;
    default:
        return nullptr;
    }
}

Element* SolarSystem::getElementFromName(ObjectTypes type, std::string name) {
    switch (type) {
    case STAR:
        return elements[starMap[name]];
    case PLANET:
        return elements[planetMap[name]];
    default:
        return nullptr;
    }
}