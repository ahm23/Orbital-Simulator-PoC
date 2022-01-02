#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Object.h"
#include <iomanip>
#include "utils_math.h"
#include "Planet.h"
#include "VertexBuffer.h";
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include <vector>
#include <fstream>
#include "Renderer.h"
#include "Texture.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "SphereRender.h"
#include "utils_debug.h"
#include "gui.hpp"

using namespace std;

Star initializeStar(string name, long mass) {
    return Star(name, mass);
}

void initializePlanet() {
    Vector3d Position;
    Vector3d Velocity;

    Planet earth(5972200000000000000000000.0, 1.326 * pow(10, 11), 0.4101524);

    earth.orbit.initOrbitCOE_R(147125124.453, 149597870.700, 0.01673, 0.00005, -0.19653524388, 1.79676742117, &Position, &Velocity);
    earth.initKineticProcess(Position, Velocity);
}

int main() {
    Star sun = Star("Sun", 1988470000000000000000000000000.0);
    initializePlanet();
    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    initGUI();

    glfwTerminate();

    return 0;
}