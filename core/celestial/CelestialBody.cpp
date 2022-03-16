#include "CelestialBody.h"
#include "../../gui/utils_render.h"

CelestialBody::CelestialBody(ObjectConfig config_obj, CelestialConfig config_clst) : Object{ config_obj }, config_clst{ config_clst } {
	g_sphere(config_clst.avg_radius, &config_obj.gpu_mesh.data, &config_obj.gpu_mesh.indicies);
};
