#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "core/celestial/Planet.h"
/*
#include "gui_old/VertexBuffer.h"
#include "gui_old/IndexBuffer.h"
#include "gui_old/VertexArray.h"
#include "gui_old/Shader.h"
*/
#include <vector>
#include <Eigen/Dense>
#include <fstream>
//#include "gui_old/Renderer.h"
//#include "gui_old/Texture.h"
//#include "gui_old/utils_debug.h"

#include "gtc/matrix_transform.hpp"
//#include "gui_old/SphereRender.h"

#include "glm.hpp"
//#include "gui_old/gui.hpp"

#include "core/SolarSystem.h"
#include "gui/GUI.h"


using namespace Eigen;


int main() {

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return 1;
	}

    GUI g;

    g.testFunc();
    //initGUI();
    SolarSystem system;

    //double position[3] = {0, -7161.8, 0};
    //double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);
	do {
        g.render();

	} while (!glfwWindowShouldClose(g.window));

    glfwTerminate();

    return 0;
}