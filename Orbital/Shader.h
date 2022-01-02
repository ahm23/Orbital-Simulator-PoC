#pragma once
#include <iostream>
#include <unordered_map>
#include <glm.hpp>

using namespace std;

class Shader
{
public:
	unsigned int id;
	string fn_vertex;
	string fn_fragment;
	unordered_map<string, int> locationCache;

	Shader(const std::string& filename_vertex, const std::string& filename_fragment);
	~Shader();

	void bind() const;
	void unbind() const;

	void SetUniform1i(const std::string& name, int v);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 &matrix);
private:
	std::string loadShader(const string &filepath);
	unsigned int CompileShader(unsigned int type, const string &source);
	unsigned int CreateShader(const string& vertexShader, const string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

