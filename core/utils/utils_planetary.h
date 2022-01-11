#pragma once

#include <Eigen/Dense>
#include "../Element.h"

static Eigen::Matrix3d generateEclipticRotation(float ecliptic) {
    ecliptic *= (float)d2r;
    Matrix3d C1{
        { 1, 0, 0 },
        {0, cos(ecliptic), sin(ecliptic)},
        {0, -sin(ecliptic), cos(ecliptic)}
    };
    return C1;
}

static Eigen::Vector3d getVectorBetweenObject(Element* o1, Element* o2) {

    int max_len = static_cast<int>(std::min(o1->depth_map.size(), o2->depth_map.size()));

    Eigen::Vector3d pos1;
    Eigen::Vector3d pos2;
    pos1 << 0, 0, 0;
    pos2 << 0, 0, 0;

    for (int i = 0; i < max_len; i++) {
        if (o1->depth_map_reverse.find(o2->depth_map[i]) != o1->depth_map_reverse.end()) {
            //found o2 @ i in o1
            //std::cout << "Found Connection @ " << o2->depth_map[i]->getName() << std::endl;
            for (int j = i; j <= o1->depth_map_reverse[o2->depth_map[i]]; j++) {
                pos1 = generateEclipticRotation(o1->depth_map[j]->getEcliptic()) * o1->depth_map[j]->orbit.getRotation() * pos1 + o1->depth_map[j]->getPos();
                if (o1->depth_map[j] == o2->depth_map[i])
                    return pos1 - pos2;
                //pos1 += getElementFromName(o1->depth_map[j]->getType(), o1->depth_map[j]->getName())->kinematic->p;
            }
        }
        else if (o2->depth_map_reverse.find(o1->depth_map[i]) != o2->depth_map_reverse.end()) {
            //found o1 @ i in o2
            //std::cout << "Found Connection @ " << o2->depth_map[i]->getName() << std::endl;
            for (int j = 0; j < o2->depth_map.size(); j++) {
                pos2 = generateEclipticRotation(o2->depth_map[j]->getEcliptic()) * o2->depth_map[j]->orbit.getRotation() * pos2 + o2->depth_map[j]->getPos();
                if (o2->depth_map[j] == o1->depth_map[i])
                    return pos1 - pos2;
            }
        }
        else {
            // (current position + next position) * rotation matrix
            pos1 = generateEclipticRotation(o1->depth_map[i]->getEcliptic()) * o1->depth_map[i]->orbit.getRotation() * pos1 + o1->depth_map[i]->getPos();
            pos2 = generateEclipticRotation(o2->depth_map[i]->getEcliptic()) * o2->depth_map[i]->orbit.getRotation() * pos2 + o2->depth_map[i]->getPos();
        }
        // TODO if not found
    }
    return pos1 - pos2;
}