#pragma once

#include <iostream>
#include <fstream>

#include "../celestial/Star.h"

template<typename T>
void dp_destruct(T** dp, int count) {
    for (int i = 0; i < count; i++)
        free(dp[i]);
    free(dp);
}


StarConfigPackage** parseStars(int* starCount) {
    std::ifstream config("C:\\Users\\netagive\\Desktop\\Orbital\\Orbital\\core\\utils\\Stars.txt");
    const int count = std::count(std::istreambuf_iterator<char>(config), std::istreambuf_iterator<char>(), '\n');

    StarConfigPackage** starList = (StarConfigPackage**)malloc(sizeof(StarConfigPackage) * count);

    if (!config) {
        std::cerr << "File can't be opened! " << std::endl;
        system("PAUSE");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        StarConfigPackage* stars = new StarConfigPackage;
        config >> stars->config_obj.name >> stars->config_obj.mass >> stars->config_obj.mu
            >> stars->config_star.avg_radius >> stars->config_star.temperature;
        starList[i] = stars;
    }
    *starCount = count;
    return starList;
}