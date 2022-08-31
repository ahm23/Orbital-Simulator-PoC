#pragma once
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <ext.hpp>

#include "utils_debug.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "SphereRender.h"

struct VRAM_DAT {
	std::vector<float> data;
	std::vector<unsigned int> indicies;
};

class GUI {
public:
	GUI(GLFWwindow*);

	bool render();

	GLFWwindow* window;

private:

	float cameraRadius = 3.0f;

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(cameraRadius, 0, 0);
	glm::vec3 positionModifier = glm::vec3(3, 0, 0);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 0;
	// Initial vertical angle : none
	float verticalAngle = 3.14f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 1.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	GLFWwindow* createWindow();

	void swapBuffers(VRAM_DAT);			// TODO
	

	Shader shader = Shader("shaders/vertex.shader", "shaders/fragment.shader");
	Texture texture = Texture("highres.jpg");
	SphereRender sphere = SphereRender(1, 100, 100);
	IndexBuffer ib = IndexBuffer(sphere.getIndices(), sphere.getIndexCount());

	void computeMatrices(GLFWwindow*);
};
