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
#include <vector>
#include <fstream>

using namespace std;


static string loadShader(const string filepath) {
    std::ifstream stream(filepath);
    string line;
    stringstream s;
    while (getline(stream, line)) {
        s << line << '\n';
    }

    return s.str();
}

static unsigned int CompileShader(unsigned int type, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int len;
        glGetShaderiv(id, GL_COMPILE_STATUS, &len);
        char* message = (char*)malloc(len*sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    
    // TODO: Error Handling

    return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        VertexBuffer buffer_v(positions, sizeof(positions));

        /*
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        */

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

        IndexBuffer buffer_i(indicies, 6);
        /*
        unsigned int index_buffer;
        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
        */

        unsigned int shader = CreateShader(loadShader("gui/shaders/vertex.shader"), loadShader("gui/shaders/fragment.shader"));
        glUseProgram(shader);

        int l = glGetUniformLocation(shader, "u_Color");
        glUniform4f(l, 249.0 / 255.0, 215.0 / 255.0, 28.0 / 255.0, 1.0);

        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(shader);

            glBindVertexArray(vao);
            buffer_i.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        glDeleteProgram(shader);
    }
    glfwTerminate();

    return 0;
}