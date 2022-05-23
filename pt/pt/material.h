#include "ray.h"
#include "hit_record.h"
#include "random.h"

struct material {
	virtual bool scatter(const ray &r, const hit_record &rec, vec3 &attenuation, ray &scattered) = 0;
};

struct lambertian : public material {
	lambertian(const vec3 &a) : color(a) {}
	bool scatter(const ray &r, const hit_record &rec, vec3 &attenuation, ray &scattered) {
		vec3 p = r.at(rec.t);
		vec3 target = p + rec.normal + random_in_unit_sphere();
		scattered = ray(p, target - p);
		attenuation = color;
		return true;
	}

	vec3 color;
};

class metal : public material {
public:
	metal(const vec3 &a, float f) : color(a), fuzz(f > 1.0f ? 1.0f : f) {};
	bool scatter(const ray &r, const hit_record &rec, vec3 &attenuation, ray &scattered) {
		vec3 reflected = reflect(normalize(r.direction), rec.normal);
		vec3 p = r.at(rec.t);
		scattered = ray(p, reflected + fuzz * random_in_unit_sphere());
		attenuation = color;
		return dot(scattered.direction, rec.normal) > 0;
	}
	vec3 color;
	float fuzz;
};