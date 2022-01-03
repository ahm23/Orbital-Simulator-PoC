#include "Star.h"

Star::Star(StarConfigPackage config) : CelestialBody{ config.config_obj } {
	this->config = config.config_star;
	luminosity = 4*PI*pow(config.config_star.avg_radius,2)*stefan_boltzmann*pow(config.config_star.temperature,4);
}
