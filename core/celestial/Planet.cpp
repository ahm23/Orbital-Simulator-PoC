#include "Planet.h"

Planet::Planet(PlanetConfigPackage config) : CelestialBody{ config.config_obj, config.config_clst } {};

Planet::~Planet() {
	std::cout << "Destructing Planet: " << config_obj.name << std::endl;
}