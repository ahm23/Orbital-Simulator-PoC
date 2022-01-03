#pragma once

#include <iostream>
#include <fstream>

#include "../celestial/Star.h"
#include "../celestial/Planet.h"

template<typename T>
void dp_destruct(T** dp, int count) {
    for (int i = 0; i < count; i++)
        free(dp[i]);
    free(dp);
}

class FileParser {
public:
    FileParser(std::string filepath) {
        file.open(filepath);
        if (!file)
            std::cerr << "File can't be opened! " << std::endl;
        file >> count;
    };

    int getCount() { return count; }

    template<typename T>
    void parse(T** var) { std::cout << "Unknown Object Type!"; }
    template<>
    void parse<Star>(Star** list) {
        for (int i = 0; i < count; i++) {
            StarConfigPackage star{};
            file >> star.config_obj.name >> star.config_obj.mass >> star.config_obj.mu
                >> star.config_clst.avg_radius >> star.config_star.temperature;
            std::cout << star.config_obj.name << endl;
            list[i] = new Star(star);
        }
    }
    template<>
    void parse<Planet>(Planet** list) {
        for (int i = 0; i < count; i++) {
            PlanetConfigPackage planet;
            file >> planet.config_obj.name >> planet.config_obj.mass >> planet.config_obj.mu
                >> planet.config_clst.avg_radius;
            list[i] = new Planet(planet);
        }
    }

private:
    std::ifstream file;
    int count;
};


/*StarConfigPackage** parseStars(int* starCount) {
    std::ifstream config("C:\\Users\\netagive\\Desktop\\Orbital\\Orbital\\core\\utils\\Stars.txt");
    const int count = std::count(std::istreambuf_iterator<char>(config), std::istreambuf_iterator<char>(), '\n');

    StarConfigPackage** starList = (StarConfigPackage**)malloc(sizeof(StarConfigPackage) * count);


    for (int i = 0; i < count; i++) {
        StarConfigPackage* stars = new StarConfigPackage;
        config >> stars->config_obj.name >> stars->config_obj.mass >> stars->config_obj.mu
            >> stars->config_star.avg_radius >> stars->config_star.temperature;
        starList[i] = stars;
    }
    *starCount = count;
    return starList;
}*/