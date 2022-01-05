#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "core/celestial/Planet.h"
#include "VertexBuffer.h";
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include <vector>
#include <Eigen/Dense>

#include <fstream>
#include "Renderer.h"
#include "Texture.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "SphereRender.h"
#include "utils_debug.h"
#include "gui.hpp"

#include "core/Object.h"
#include "core/celestial/Star.h"
#include "core/utils/utils_parse.h"
#include "core/Sattelite.h"

using namespace std;
using namespace Eigen;

std::string* planetMap;
std::string* starMap;

std::vector<Star*> loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    std::vector<Star*> starList;
    parser.parseObjects<Star>(&starList);
    starMap = new std::string[parser.getCount()];
    for (int i = 0; i < starList.size(); i++)
        starMap[i] = starList[i]->getName();
    return starList;
}

std::vector<Planet*> loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    std::vector<Planet*> planetList;
    //Planet** planetList = (Planet**)malloc(sizeof(Planet) * parser.getCount());
    parser.parseObjects<Planet>(&planetList);
    return planetList;
}

std::vector<Sattelite*> loadSattelites() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Sattelites.dat");
    std::vector<Sattelite*> satteliteList;
    parser.parseObjects<Sattelite>(&satteliteList);
    return satteliteList;
}

/*Meteor** loadMeteors() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\Orbital\\core\\utils\\Meteors.dat");
    Meteor** meteorList = (Meteor**)malloc(sizeof(MeteorConfigPackage) * parser.getCount());
    parser.parse<Meteor>(meteorList);
    return meteorList;
}*/

/*Comet** loadComets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\Orbital\\core\\utils\\Comets.dat");
    Comet** cometList = (Comet**)malloc(sizeof(CometConfigPackage) * parser.getCount());
    parser.parse<Comet>(cometList);
    return cometList;
}*/

void initializePlanetaryOrbit(int num, Planet* planet, std::vector<Star*>* starList) {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    
    Eigen::Vector3d Position, Velocity;
    std::string ref_object;

    OrbitInit init = parser.parseOrbit(num, planet, &ref_object);

    init.init_mu = calculate_mu(planet->getMass(), (starList[0][0])->getMass());

    planet->setMu(init.init_mu);

    switch (init.type) {
        case 0:
            planet->orbit.initOrbitCOE_TA(init, &Position, &Velocity);
            break;
        case 1:
            planet->orbit.initOrbitCOE_R(init, &Position, &Velocity);
            break;
        case 2:
            //planet->orbit.initOrbitCOE_R(init, &Position, &Velocity);
            break;
        case 3:
            planet->orbit.initOrbitCOE_ML(init, &Position, &Velocity);
            break;
    }
    planet->initKineticProcess(Position, Velocity);
}



void initializeSattelites() {

}


int main() {
    std::vector<Star*> starList = loadStars();
    std::vector<Planet*> planetList = loadPlanets();

    for (int i = 0; i < planetList.size(); i++) {
        initializePlanetaryOrbit(i + 1, planetList[i], &starList);
    }



    //initializePlanet("Earth", 5972200000000000000000000.0, 1.326 * pow(10, 11), 0.4101524);
    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    initGUI();

    glfwTerminate();

    return 0;
}