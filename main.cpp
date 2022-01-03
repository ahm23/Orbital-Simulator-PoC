#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "utils_math.h"
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

Star** loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Stars.dat");
    Star** starList = (Star**)malloc(sizeof(StarConfigPackage) * parser.getCount());
    parser.parse<Star>(starList);
    return starList;
}

Planet** loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Planets.dat");
    Planet** planetList = (Planet**)malloc(sizeof(PlanetConfigPackage) * parser.getCount());
    parser.parse<Planet>(planetList);
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

/*Planet* initializePlanet(string name, double mass, double gravParam, float inclination_ecliptic) {
    Eigen::Vector3d Position, Velocity;
    Planet* planet = new Planet(name, mass, gravParam, 0.4101524, );
    planet->orbit.initOrbitCOE_R(147125124.453, 149597870.700, 0.01673, 0.00005, -0.19653524388, 1.79676742117, &Position, &Velocity);
    planet->initKineticProcess(Position, Velocity);
    return planet;
}*/



int main() {
    //Star sun = Star("Sun", 1988470000000000000000000000000.0);
    loadStars();
    //initializePlanet("Earth", 5972200000000000000000000.0, 1.326 * pow(10, 11), 0.4101524);
    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    initGUI();

    glfwTerminate();

    return 0;
}