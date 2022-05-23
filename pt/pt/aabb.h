#pragma once
#include "vec3.h"
struct AABB {
	vec3 min_xyz, max_xyz;
	AABB() {}
	AABB(const vec3 &a, const vec3 &b):min_xyz(a), max_xyz(b) {}
	bool intersect(ray &r) {
		float tx1 = (max_xyz.x - r.origin.x) / r.direction.x;
		float tx2 = (min_xyz.x - r.origin.x) / r.direction.x;
		float ty1 = (max_xyz.y - r.origin.y) / r.direction.y;
		float ty2 = (min_xyz.y - r.origin.y) / r.direction.y;
		float tz1 = (max_xyz.z - r.origin.z) / r.direction.z;
		float tz2 = (min_xyz.z - r.origin.z) / r.direction.z;

		if (tx1 > tx2) std::swap(tx1, tx2);
		if (ty1 > ty2) std::swap(ty1, ty2);
		if (tz1 > tz2) std::swap(tz1, tz2);

		float L = std::max(std::max(tx1, ty1), tz1);
		float R = std::min(std::min(tx2, ty2), tz2);
		if (L > R) return 0;
		return 1;
	}
};