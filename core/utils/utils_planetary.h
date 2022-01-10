#pragma once

#include <Eigen/Dense>
#include "../Element.h"

Eigen::Vector3d getVectorBetweenObject(Element* o1, Element* o2) {
     std::unordered_map<int, Object*>* map_long;
     std::unordered_map<int, Object*>* map_short;


     if (o2->depth_map.size() > o1->depth_map.size()) {
         map_long = &o2->depth_map;
         map_short = &o1->depth_map;
     }
     else {
         map_long = &o1->depth_map;
         map_short = &o2->depth_map;
     }
     std::unordered_map<Object*, Eigen::Vector3d> dist_map_1;
     std::unordered_map<Object*, Eigen::Vector3d> dist_map_2;
     int max_len = (std::max)(o1->depth_map.size(), o2->depth_map.size());

    Eigen::Vector3d pos1;
    Eigen::Vector3d pos2;
    pos1 << 0, 0, 0;
    pos2 << 0, 0, 0;


    for (int i = 0; i < map_short->size(); i++) {
        if (o1->depth_map_reverse.find(o2->depth_map[i]) != o1->depth_map_reverse.end()) {
            //found o2 @ i in o1
            std::cout << "Found Connection @ " << o2->depth_map[i] << std::endl;
            for (int j = i; j <= o1->depth_map_reverse[o2->depth_map[i]]; j++) {
                pos1 << (pos1 + o1->depth_map[j]->getPos()) * 1;
                if (o1->depth_map[j] == o2->depth_map[i])
                    return pos1 - pos2;
                //pos1 += getElementFromName(o1->depth_map[j]->getType(), o1->depth_map[j]->getName())->kinematic->p;
            }
        }
        else if (o2->depth_map_reverse.find(o1->depth_map[i]) != o2->depth_map_reverse.end()) {
            //found o1 @ i in o2
            std::cout << "Found Connection @ " << o2->depth_map[i] << std::endl;
            for (int j = 0; j < o2->depth_map.size(); j++) {
                pos2 = (pos2 + o2->depth_map[j]->getPos()) * 1;
                if (o2->depth_map[j] == o1->depth_map[i])
                    return pos2 - pos1;
            }
        }
        else {
            // (current position + next position) * rotation matrix
            pos1 = (pos1 + o1->depth_map[i]->getPos()) * 1;
            pos2 = (pos2 + o2->depth_map[i]->getPos()) * 1;
        }
    }
    return pos1;
}