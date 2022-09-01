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
#include "stb_image.h"
#include "VRAMPipeline.h"

struct VRAM_DATA {
	std::vector<float> data;
	std::vector<unsigned int> indicies;
};

class GUI {
public:
	GUI();


	void testFunc();

	bool render();

	GLFWwindow* window = createWindow();

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

	glm::mat4 getViewMatrix() {
		return ViewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return ProjectionMatrix;
	}



	GLFWwindow* createWindow();

	void swapBuffers(VRAM_DATA);			// TODO
	
	GLFWimage icons[1];

	VRAMPipeline pipe = VRAMPipeline();

	Shader* shader;
	Texture* texture;


	void computeMatrices(GLFWwindow*);
};
