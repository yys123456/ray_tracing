#include "vec3.h"
#include <random>
std::random_device rd;
float rand01() {
	return float(rd()) / (float(rd.max()) + 1.0f);
}
vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(rand01(), rand01(), rand01()) - vec3(1.0, 1.0, 1.0);
	} while (length(p) >= 1.0);
	return p;
}