#pragma once

#include <iostream>
#include <fstream>

#include "../celestial/Star.h"
#include "../celestial/Planet.h"
#include "../Sattelite.h"
#include <limits>

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
    void parseObjects(std::vector<T*>* var) { std::cout << "Unknown Object Type!"; }
    template<>
    void parseObjects<Star>(std::vector<Star*>* list) {
        for (int i = 0; i < count; i++) {
            StarConfigPackage star{};
            file >> star.config_obj.name >> star.config_obj.mass >> star.config_obj.mu
                >> star.config_clst.avg_radius >> star.config_star.temperature;
            list->push_back(new Star(star));
        }
    }
    template<>
    void parseObjects<Planet>(std::vector<Planet*>* list) {
        for (int i = 0; i < count; i++) {
            PlanetConfigPackage planet;
            std::string line;
            file >> planet.config_obj.name >> planet.config_obj.mass >> planet.config_obj.mu
                >> planet.config_clst.avg_radius >> planet.config_planet.ecliptic_i >> line;
            std::getline(file, line, '\n');
            list->push_back(new Planet(planet));
        }
    }
    template<>
    void parseObjects<Sattelite>(std::vector<Sattelite*>* list) {
        for (int i = 0; i < count; i++) {
            SatteliteConfigPackage sattelite;
            file >> sattelite.config_obj.name >> sattelite.config_obj.mass >> sattelite.config_obj.mu;
            std::string line; std::getline(file, line, '\n');
            std::getline(file, line, '\n');
            list->push_back(new Sattelite(sattelite));
        }
    }


    OrbitInit parseOrbit(int num, Planet* planet, std::string* ref) {
        //std::cout << num << endl;
        for (int i = 0; i <= 2 * num - 1; ++i) {
            std::string line; std::getline(file, line, '\n');
        }
        OrbitInit init;
        file >> init.type;
        file >> *ref;
        switch (init.type) {
            case 0:
                file >> init.coe.TA >> init.coe.a >> init.coe.e >> init.coe.i >> init.coe.raan >> init.coe.omega;
                return init;
            case 1:
                file >> init.init_r >> init.coe.a >> init.coe.e >> init.coe.i >> init.coe.raan >> init.coe.omega;
                return init;
            case 2:
                file >> init.init_ma >> init.coe.e >> init.coe.i >> init.coe.raan >> init.coe.omega;
                return init;
            case 3:
                file >> init.coe.a >> init.coe.e >> init.coe.i >> init.coe.raan >> init.coe.omega >>  init.init_ml;
                init.coe.i = init.coe.i * d2r;
                init.coe.raan = init.coe.raan * d2r;
                init.coe.omega = init.coe.omega * d2r;
                init.init_ml = init.init_ml * d2r;
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