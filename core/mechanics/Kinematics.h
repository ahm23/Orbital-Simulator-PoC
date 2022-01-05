#pragma once
#include <Eigen/Dense>
class Object;

struct Kinematic {
	Object* anchor;
	Eigen::Vector3d p;
	Eigen::Vector3d v;
	Eigen::Vector3d a;
};