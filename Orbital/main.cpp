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
    const int star_count = 1;
    vector<Star> stars;
    stars.push_back(Star("Sun", 1988470000000000000000000000000.0));
    //initializePlanet();
    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };
    //Object satellite(100.0, position, velocity);

    initGUI();
    /*
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "A M OGU S", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!";

    {

        GL(glEnable(GL_BLEND));
        GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        SphereRender sphere(100, 38, 18);

        Shader shader("gui/shaders/vertex.shader", "gui/shaders/fragment.shader");
        shader.bind();
        //shader.SetUniform4f("u_Color", 249.0 / 255.0, 215.0 / 255.0, 28.0 / 255.0, 1.0);

        VertexArray va;
        VertexBuffer vb(sphere.getBufferData(), sphere.getBufferDataSize());
        
        BufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(3);
        layout.push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(sphere.getIndices(), sphere.getIndexCount());

        // debug
        std::cout << sphere.getVerticiesSize() << endl;
        std::cout << sphere.getIndexCount() << endl;

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(960.0 / 540.0), 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.5, 0, 0), // Camera is at (4,3,3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

        glViewport(0, 0, (GLsizei)960.0, (GLsizei)540.0);

        glm::mat4 model = glm::mat4(1.0);

        glm::mat4 mvp = proj * view * model;

        shader.SetUniformMat4f("matrixModelView", view);
        glm::mat4 matrixNormal = view;
        matrixNormal[3] = glm::vec4(0, 0, 0, 1);
        shader.SetUniformMat4f("matrixNormal", matrixNormal);
        shader.SetUniformMat4f("matrixModelViewProjection", mvp);

        Texture texture("earth2048.bmp");
        texture.Bind();
        //shader.SetUniform1i("u_Texture", 0);
        
        Renderer renderer;

        while (!glfwWindowShouldClose(window)) {
            renderer.Clear();
            renderer.Draw(va, ib, shader);
            GL(glfwSwapBuffers(window));
            GL(glfwPollEvents());
        }
    }*/
    glfwTerminate();

    return 0;
}