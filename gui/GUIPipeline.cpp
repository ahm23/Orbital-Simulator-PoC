#include "GUIPipeline.h"

GUIPipeline::GUIPipeline(double initFoV)
{
	FoV = initFoV;
	dpp = initFoV/1920;
}
