#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Object.h"


int main() {


    double position[3] = {0, -7161.8, 0};
    double velocity[3] = { -1.0749, 0, 7.3862 };

    Object satellite(100.0, position, velocity);

    while (true) {
        double *position = satellite.getPosition();

        for (int i = 3 - 1; i >= 0; i--) {
            Sleep(1);
            //cout << position[i];
        }
        
    }

    return EXIT_SUCCESS;
}