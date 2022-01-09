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

    template<typename T, typename X>
    void parseObjects(std::vector<T*>* var) { std::cout << "Unknown Object Type!"; }
    template<>
    void parseObjects<Element, Star>(std::vector <Element*>* list) {
        for (int i = 0; i < count; i++) {
            StarConfigPackage star;
            star.config_obj.type = ObjectTypes::STAR;
            file >> star.config_obj.name >> star.config_obj.mass >> star.config_obj.mu
                >> star.config_clst.avg_radius >> star.config_star.temperature;
            Element* el = new Element;
            el->obj = new Star(star);
            list->push_back(el);
        }
    }
    template<>
    void parseObjects<Element, Planet>(std::vector<Element*>* list) {
        for (int i = 0; i < count; i++) {
            PlanetConfigPackage planet;
            std::string line;
            planet.config_obj.id = i;
            planet.config_obj.type = ObjectTypes::PLANET;
            file >> planet.config_obj.name >> planet.config_obj.mass >> planet.config_obj.mu
                >> planet.config_clst.avg_radius >> planet.config_planet.ecliptic_i >> line;
            std::getline(file, line, '\n');
            Element* el = new Element;
            el->obj = new Planet(planet);
            list->push_back(el);
        }
    }
    template<>
    void parseObjects<Element, Sattelite>(std::vector<Element*>* list) {
        for (int i = 0; i < count; i++) {
            SatteliteConfigPackage sattelite;
            sattelite.config_obj.type = ObjectTypes::SATTELITE;
            file >> sattelite.config_obj.name >> sattelite.config_obj.mass >> sattelite.config_obj.mu;
            std::string line; std::getline(file, line, '\n');
            std::getline(file, line, '\n');
            Element* el = new Element;
            el->obj = new Sattelite(sattelite);
            list->push_back(el);
        }
    }


    OrbitInit parseOrbit(int num, Object* planet, int* ref_t, std::string* ref_o) {
        //std::cout << num << endl;
        for (int i = 0; i <= 2 * num - 1; ++i) {
            std::string line; std::getline(file, line, '\n');
        }
        OrbitInit init;
        file >> init.type;
        file >> *ref_t;
        file >> *ref_o;
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
            default:
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