#include "Object.h"

Object::Object(ObjectConfig config_obj) {
	this->config_obj = config_obj;

}

Object::~Object() {
	std::cout << "Destructing Object: " << config_obj.name << std::endl;
}