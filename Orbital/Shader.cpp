#include "Shader.h"
#include <fstream>
#include <sstream>
#include <GL/glew.h>

using namespace std;

Shader::Shader(const string& filename_vertex, const string& filename_fragment): fn_vertex(filename_vertex), fn_fragment(filename_fragment), id(0)
{
    id = CreateShader(loadShader(filename_vertex), loadShader(filename_fragment));
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::bind() const
{
    glUseProgram(id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), 249.0 / 255.0, 215.0 / 255.0, 28.0 / 255.0, 1.0);
}

std::string Shader::loadShader(const string &filepath) {
    std::ifstream stream(filepath);
    string line;
    stringstream s;
    while (getline(stream, line)) {
        s << line << '\n';
    }

    return s.str();
}

unsigned int Shader::CompileShader(unsigned int type, const string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int len;
        glGetShaderiv(id, GL_COMPILE_STATUS, &len);
        char* message = (char*)malloc(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << message << endl;
        glDeleteShader(id);
        return 0;
    }


    // TODO: Error Handling

    return id;
}

unsigned int Shader::CreateShader(const string& vertexShader, const string& fragmentShader) {
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


int Shader::GetUniformLocation(const std::string& name)
{
    if (locationCache.find(name) != locationCache.end())
        return locationCache[name];

    int location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist." << endl;
        locationCache[name] = location;
    }
    return location;
}

