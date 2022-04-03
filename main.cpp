#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "core/celestial/Planet.h"
#include "gui/VertexBuffer.h"
#include "gui/IndexBuffer.h"
#include "gui/VertexArray.h"
#include "gui/Shader.h"

#include <vector>
#include <Eigen/Dense>
#include <fstream>
#include "gui/Renderer.h"
#include "gui/Texture.h"

#include "gtc/matrix_transform.hpp"
#include "gui/SphereRender.h"
#include "gui/utils_debug.h"

#include "glm.hpp"
#include "gui/gui.hpp"

#include "core/SolarSystem.h"


using namespace std;
using namespace Eigen;

int main() {

    SolarSystem system;

    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    initGUI();

    glfwTerminate();

    return 0;
}