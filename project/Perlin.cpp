#include <libnoise/noise.h>
#include "Perlin.h"

noise::module::Perlin Perlin::perlin;

float Perlin::getPerlinValue(float x, float y)
{
    return (float)perlin.GetValue(x, y, 0.75);
}