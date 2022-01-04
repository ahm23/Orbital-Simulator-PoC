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
    void parseObjects(T** var) { std::cout << "Unknown Object Type!"; }
    template<>
    void parseObjects<Star>(Star** list) {
        for (int i = 0; i < count; i++) {
            StarConfigPackage star{};
            file >> star.config_obj.name >> star.config_obj.mass >> star.config_obj.mu
                >> star.config_clst.avg_radius >> star.config_star.temperature;
            list[i] = new Star(star);
        }
    }
    template<>
    void parseObjects<Planet>(Planet** list) {
        for (int i = 0; i < count; i++) {
            PlanetConfigPackage planet;
            file >> planet.config_obj.name >> planet.config_obj.mass >> planet.config_obj.mu
                >> planet.config_clst.avg_radius >> planet.config_planet.ecliptic_i;
            file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen('\n'));
            list[i] = new Planet(planet);
        }
    }

    OrbitInit parseOrbit(int num, Planet* planet, std::string* ref) {
        //std::cout << num << endl;
        for (int i = 0; i <= num*(num + 1) + 1; ++i)
            file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen('\n'));
        OrbitInit init;
        file >> init.type;
        file >> *ref;
        switch (init.type) {
            case 0:
                file >> init.coe.TA >> init.coe.a >> init.coe.e >> init.coe.i >> init.coe.omega >> init.coe.raan;
                return init;
            case 1:
                file >> init.init_r >> init.coe.a >> init.coe.e >> init.coe.i >> init.coe.omega >> init.coe.raan;
                return init;
        }
    }

private:
    std::ifstream file;
    int count;
};

enum OrbitPrelimMethod {
    COE_R = 0,
    COE_T = 1
};