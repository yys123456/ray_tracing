#include "camera.h"
#include "bvh.h"
#include "attribute.h"
#include "stl_reader.h"

#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

vector<primitive *> scene;
vector<attribute *> attributes;
BVHnode *bvh;

vec3 render(ray &r, int depth, int max_depth) {
	if (depth >= max_depth) {
		return vec3(0, 0, 0);
	}
	hit_record rec;
	if (intersect(bvh, r, rec, scene)) {
		ray scattered;
		vec3 attenuation;
		if (attributes[rec.attr_id]->mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * render(scattered, depth + 1, max_depth);
		}
		return vec3(0, 0, 0);
	}
	vec3 unit_direction = normalize(r.direction);
	float t = 0.5 * (unit_direction.y + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

void read_mesh(const string &path, int a) {
	stl_reader::StlMesh <float, unsigned int> mesh(path);
	for (size_t itri = 0; itri < mesh.num_tris(); ++itri) {
		vec3 p[3];
		for (size_t icorner = 0; icorner < 3; ++icorner) {
			const float* c = mesh.tri_corner_coords(itri, icorner);
			p[icorner] = { c[0], c[1], c[2] };
		}
		scene.push_back(new triangle(p[0], p[1], p[2], a));
		(scene.back())->name = string("triangle") + to_string(itri) + "_" + path;
	}
}

int main() {
	int width = 1000, height = 500;
	ofstream ofs("pic.ppm");
	ofs << "P3\n" << width << " " << height << "\n255\n";

	camera cam(90, float(width) / height, vec3(0, 0, 1.5), vec3(0, 0, -1), vec3(0, 1, 0));

	attributes.push_back(new attribute(new lambertian(vec3(0.5, 0.5, 0))));
	attributes.push_back(new attribute(new metal(vec3(0.5, 0.5, 0.5), 0.05)));

	//scene.push_back(new sphere(vec3(0, -1000.5, 0), 1000, 1));
	//scene.push_back(new sphere(vec3(0, 0, 0), 0.5, 1));
	//read_mesh("cone.stl", 0);
	read_mesh("teapot.stl", 1);
	read_mesh("plane.stl", 0);
	bvh = build(0, scene.size() - 1, scene);

	for (int j = height - 1; j >= 0; --j) {
		cerr << "\rscanlines remain: " << j << ' ' << flush;
		for (int i = 0; i < width; ++i) {
			vec3 color;
			for (int k = 0; k < 50; k++) {
				float u = (i + rand01()) / float(width - 1);
				float v = (j + rand01()) / float(height - 1);
				ray r = cam.get_ray(u, v);
				vec3 col = render(r, 0, 10);
				color = color + col;
			}
			color = sqrt(color / 100.0f) * 255.99f;
			ofs << int(color.x) << ' ' << int(color.y) << ' ' << int(color.z) << endl;
		}
	}
	ofs.close();
	getchar();
}



 