#include "ray.h"
#include "hit_record.h"
#include <string>

#define INF 1e18

struct primitive {
public:
	int attr_id;
	std::string name;
	virtual vec3 center() const = 0;
	virtual bool intersect(ray &r, hit_record &rec) = 0;
	virtual vec3 min_xyz() = 0;
	virtual vec3 max_xyz() = 0;
};

struct triangle : primitive {
	vec3 p[3];
	triangle(const vec3 &a, const vec3 &b, const vec3 &c, int attr){
		p[0] = a;
		p[1] = b;
		p[2] = c;
		attr_id = attr;
	}
	vec3 center() const {
		return (p[0] + p[1] + p[2]) / 3.0;
	}
	bool intersect(ray &r, hit_record &rec) {
		const float EPSILON = 0.0000001;
		vec3 edge1, edge2, h, s, q;
		float a, f, u, v;
		edge1 = p[1] - p[0];
		edge2 = p[2] - p[0];
		h = cross(r.direction, edge2);
		a = dot(edge1, h);
		if (a > -EPSILON && a < EPSILON)
			return false;    // This ray is parallel to this triangle.
		f = 1.0 / a;
		s = r.origin - p[0];
		u = f * dot(s, h);
		if (u < 0.0 || u > 1.0)
			return false;
		q = cross(s, edge1);
		v = f * dot(r.direction, q);
		if (v < 0.0 || u + v > 1.0)
			return false;
		// At this stage we can compute t to find out where the intersection point is on the line.
		float t = f * dot(edge2, q);
		if (t > EPSILON) // ray intersection
		{
			rec = { t, normalize(cross(edge1, edge2)), attr_id };
			return true;
		}
		else // This means that there is a line intersection but not a ray intersection.
			return false;
	}
	
	//bool intersect(ray &r, hit_record &rec) {
	//	vec3 normal = cross(p[1] - p[0], p[2] - p[0]);
	//	float down = dot(normal, r.direction);
	//	if (abs(down) < 1e-5) return 0;
	//	float t = -(dot(normal, r.origin) - dot(normal, p[0])) / down;
	//	vec3 point = r.at(t);
	//	vec3 p0p = point - p[0], p0p1 = p[1] - p[0];
	//	vec3 c0 = cross(p0p1, p0p);
	//	vec3 p1p = point - p[1], p1p2 = p[2] - p[1];
	//	vec3 c1 = cross(p1p2, p1p);
	//	vec3 p2p = point - p[2], p2p0 = p[0] - p[2];
	//	vec3 c2 = cross(p2p0, p2p);
	//	if (dot(c0, normal) > 0.0f && dot(c1, normal) > 0.0f && dot(c2, normal) > 0.0f) {
	//		rec = { t, normal, attr_id };
	//		return 1;
	//	}
	//	return 0;
	//}
	vec3 min_xyz() {
		vec3 res(INF, INF, INF);
		for (int i = 0; i < 3; i++) res.x = std::min(res.x, p[i].x);
		for (int i = 0; i < 3; i++) res.y = std::min(res.y, p[i].y);
		for (int i = 0; i < 3; i++) res.z = std::min(res.z, p[i].z);
		return res - vec3(0.01, 0.01, 0.01);
	}
	vec3 max_xyz() {
		vec3 res(-INF, -INF, -INF);
		for (int i = 0; i < 3; i++) res.x = std::max(res.x, p[i].x);
		for (int i = 0; i < 3; i++) res.y = std::max(res.y, p[i].y);
		for (int i = 0; i < 3; i++) res.z = std::max(res.z, p[i].z);
		return res + vec3(0.01, 0.01, 0.01);
	}
};

// 求交记录的法线务必是单位向量，因为在diffuse材质中散射光线构造会用到unit_sphere_vector, 如果法线过大，那么就会出现
// 散射光线变化不明显，并且会一直不明显下去（2、3...次反弹）这样就会造成局部出现莫名奇妙的圆环问题

struct sphere : primitive {
	vec3 o;
	float R;
	sphere(const vec3 &o, float R, int attr) :o(o), R(R) { attr_id = attr; }
	vec3 center() const {
		return o;
	}
	bool intersect(ray &r, hit_record &rec) {
		vec3 oc = r.origin - o;
		auto a = dot(r.direction, r.direction);
		auto b = 2.0 * dot(oc, r.direction);
		auto c = dot(oc, oc) - R * R;
		auto discriminant = b * b - 4 * a * c;
		if (discriminant < 0) return 0;
		float root = (-b - sqrt(discriminant)) / (2 * a);
		if (root > 0) {
			rec = { root, normalize(r.at(root) - o), attr_id };
			return 1;
		}
		root = (-b + sqrt(discriminant)) / (2 * a);
		if (root > 0) {
			rec = { root, normalize(r.at(root) - o), attr_id};
			return 1;
		}
		return 0;
	}
	vec3 min_xyz() {
		return { o.x - R, o.y - R, o.z - R };
	}
	vec3 max_xyz() {
		return { o.x + R, o.y + R, o.z + R };
	}
};
