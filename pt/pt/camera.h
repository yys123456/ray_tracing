#include <cmath>
#include "ray.h"
#define PI 3.14159265359

struct camera {
	camera(float vfov, float aspect_ratio,
		const vec3 &o, const vec3 &target, const vec3 &wup) {
		origin = o;
		world_up = wup;
		w = normalize(target - origin);
		u = normalize(cross(w, world_up));
		v = normalize(cross(u, w));

		auto radians = [](float d) {
			return d / 180.0 * PI;
		};

		float half_height = tan(radians(vfov / 2)) * length(origin - target);
		float half_width = aspect_ratio * half_height;

		lower_left = -half_width * u - half_height * v + w;
		horizontal = 2.0f * half_width * u;
		vertical = 2.0f * half_height * v;
	}

	ray get_ray(float x, float y) {
		vec3 target = lower_left + x * horizontal + y * vertical;
		return ray(
			origin, target - origin
		);
	}

	vec3 origin;
	vec3 world_up;
	vec3 u, v, w;
	vec3 horizontal, vertical;
	vec3 lower_left;
};