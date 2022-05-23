#pragma once
#include "vec3.h"
struct ray {
	vec3 origin;
	vec3 direction;
	ray(){}
	ray(const vec3 &a, const vec3 &b):origin(a), direction(b){}
	vec3 at(float t) const{
		return origin + direction * t;
	}
};