#pragma once
#include "vec3.h"
struct hit_record {
	float t;
	vec3 normal;
	int attr_id;
};