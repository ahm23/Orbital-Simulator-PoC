#pragma once
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GUI {
public:
	GUI();

	GLFWwindow* window;

private:
	GLFWwindow* createWindow();
};

