#pragma once
#include <iostream>
#include <algorithm>

struct vec3 {
	float x, y, z;
	vec3() { x = y = z = 0; }
	vec3(float a, float b, float c) :x(a), y(b), z(c) {}
};

void out(const vec3 &p) {
	std::cout << p.x << ' ' << p.y << ' ' << p.z << std::endl;
}

vec3 union_max(const vec3 &a, const vec3 &b) {
	return {
		std::max(a.x, b.x),
		std::max(a.y, b.y),
		std::max(a.z, b.z) };
}

vec3 union_min(const vec3 &a, const vec3 &b) {
	return {
		std::min(a.x, b.x),
		std::min(a.y, b.y),
		std::min(a.z, b.z) };
}

vec3 operator*(const vec3 &a, float b) {
	return { a.x * b, a.y * b, a.z * b };
}
vec3 operator*(float b, const vec3 &a) {
	return { a.x * b, a.y * b, a.z * b };
}
vec3 operator*(const vec3 &a, const vec3 &b) {
	return { a.x * b.x, a.y * b.y, a.z * b.z };
}
vec3 operator+(const vec3 &a, const vec3 &b) {
	return { a.x + b.x , a.y + b.y, a.z + b.z };
}
vec3 operator-(const vec3 &a, const vec3 &b) {
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}
vec3 operator/(const vec3 &a, float b) {
	return { a.x / b, a.y / b, a.z / b };
}
vec3 cross(const vec3 &a, const vec3 &b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}
vec3 sqrt(const vec3 &a) {
	return { sqrt(a.x), sqrt(a.y), sqrt(a.z) };
}

float length(const vec3 &a) {
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
vec3 normalize(const vec3 &a) {
	return a / length(a);
}
float dot(const vec3 &a, const vec3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
vec3 reflect(const vec3 &v, const vec3 &n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const vec3 &v, const vec3 &n, float eta, vec3 &refracted) {
	vec3 nv = normalize(v);
	float cos_theta_i = dot(nv, n);
	float d = 1.0f - eta * eta * (1.0f - cos_theta_i * cos_theta_i);
	if (d < 0.0f) return false;
	else {
		refracted = eta * nv - (eta * cos_theta_i + sqrt(d)) * n;
		return true;
	}
}