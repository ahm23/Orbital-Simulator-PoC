#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "core/utils/utils_math.h"
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

using namespace std;
using namespace Eigen;

std::string* planetMap;
std::string* starMap;

Star** loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    Star** starList = (Star**)malloc(sizeof(void*) * parser.getCount());
    parser.parseObjects<Star>(starList);
    starMap = new std::string[parser.getCount()];
    for (int i = 0; i < sizeof(*starList) / sizeof(void*); i++)
        starMap[i] = starList[i]->getName();
    return starList;
}

Planet** loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    Planet** planetList = (Planet**)malloc(sizeof(int*) * parser.getCount());
    parser.parseObjects<Planet>(planetList);
    return planetList;
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

void initializeOrbit(int num, Planet* planet, Star** starList) {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    
    Eigen::Vector3d Position, Velocity;
    std::string ref_object;

    OrbitInit init = parser.parseOrbit(num, planet, &ref_object);

    //vector<int>::iterator itr = std::find(starMap[0], starMap[4], ref_object);

    init.init_mu = calculate_mu(planet->getMass(), starList[0]->getMass());

    planet->setMu(init.init_mu);

    switch (init.type) {
        case 0:
            planet->orbit.initOrbitCOE_TA(init, &Position, &Velocity);
            break;
        case 1:
            planet->orbit.initOrbitCOE_R(init, &Position, &Velocity);
            break;
    }
    planet->initKineticProcess(Position, Velocity);
}



int main() {
    //Star sun = Star("Sun", 1988470000000000000000000000000.0);
    Star** starList = loadStars();
    Planet** planetList = loadPlanets();


    for (int i = 0; i < sizeof(*planetList)/8.0; i++) {
        initializeOrbit(i, planetList[i], starList);
    }

    //initializePlanet("Earth", 5972200000000000000000000.0, 1.326 * pow(10, 11), 0.4101524);
    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    initGUI();

    glfwTerminate();

    return 0;
}