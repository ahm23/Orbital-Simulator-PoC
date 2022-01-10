#pragma once

Eigen::Vector3d getVectorBetweenObjects(Element* o1, Element* o2) {
    /* std::unordered_map<int, Object*>* map_long;
     std::unordered_map<int, Object*>* map_short;


     if (o2->depth_map.size() > o1->depth_map.size()) {
         map_long = &o2->depth_map;
         map_short = &o1->depth_map;
     }
     else {
         map_long = &o1->depth_map;
         map_short = &o2->depth_map;
     }
     */
     //std::unordered_map<Object*, Eigen::Vector3d> map1;
    int max_len = (std::max)(o1->depth_map.size(), o2->depth_map.size());

    Eigen::Vector3d pos1;
    Eigen::Vector3d pos2;
    pos1 << 0, 0, 0;
    pos2 << 0, 0, 0;

    for (int i = 0; i < max_len; i++) {
        if (o1->depth_map_reverse.find(o2->depth_map[i]) != o1->depth_map_reverse.end())
            for (int j = 0; j < o1->depth_map.size(); j++) {
                if (o1->depth_map[j] == o2->obj)
                    return pos1;
                pos1 += getElementFromName(o1->depth_map[j]->getType(), o1->depth_map[j]->getName())->kinematic->p;
            }
        if (o2->depth_map_reverse.find(o1->depth_map[i]) != o2->depth_map_reverse.end())
            for (int j = 0; j < o2->depth_map.size(); j++) {
                if (o2->depth_map[j] == o1->obj)
                    return pos1;
                pos1 += getElementFromName(o2->depth_map[j]->getType(), o2->depth_map[j]->getName())->kinematic->p;
            }
        //map1[o1->kinematic->getKinematicAnchor()] = o1->kinematic->p;
        //map2[o2->kinematic->getKinematicAnchor()] = o2->kinematic->p;
    }
    return pos1;
}