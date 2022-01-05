#include "Sattelite.h"

Sattelite::Sattelite(SatteliteConfigPackage config) : Object { config.config_obj } {};

Sattelite::~Sattelite() {
	std::cout << "Destructing Sattelite: " << config_obj.name << std::endl;
}