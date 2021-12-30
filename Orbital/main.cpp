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
    const int star_count = 1;
    vector<Star> stars;
    stars.push_back(Star("Sun", 1988470000000000000000000000000.0));
    initializePlanet();
    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1280, "A M OGU S", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!";

    {
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f, 0.5f,
             -0.5f, 0.5f
        };

        unsigned int indicies[] = {
            0,1,2,
            2,3,0
        };

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        BufferLayout layout;

        layout.push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer buffer_i(indicies, 6);

        Shader shader("gui/shaders/vertex.shader", "gui/shaders/fragment.shader");
        shader.bind();
        shader.SetUniform4f("u_Color", 249.0 / 255.0, 215.0 / 255.0, 28.0 / 255.0, 1.0);

        va.unbind();
        shader.unbind();
        vb.unbind();
        buffer_i.unbind();

        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            shader.bind();

            va.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();

    return 0;
}