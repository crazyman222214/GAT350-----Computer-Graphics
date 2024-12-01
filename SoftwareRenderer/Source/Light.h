#pragma once
#include <glm/glm.hpp>
#include "Color.h"

enum light_type_t
{
	DIRECTIONAL,
	POINT
};

struct light_t
{
	glm::vec3 position;
	glm::vec3 direction;
	color3_t color;
	light_type_t lightType;
};
