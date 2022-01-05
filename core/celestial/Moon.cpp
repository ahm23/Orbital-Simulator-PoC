#pragma once
#include "Moon.h"

Moon::Moon(MoonConfigPackage config) : CelestialBody{ config.config_obj, config.config_clst } {};

Moon::~Moon() {
	std::cout << "Destructing Moon: " << config_obj.name << std::endl;
}