#include <iostream>
#include <vector>
#include <algorithm>

#include "aabb.h"
#include "primitive.h"

struct BVHnode {
	BVHnode *lchild, *rchild;
	AABB box;
	int l, r;
};

AABB get_box(int l, int r, std::vector<primitive *> &scene) {
	vec3 min_xyz(INF, INF, INF), max_xyz(-INF, -INF, -INF);
	for (int i = l; i <= r; i++) {
		primitive *t = scene[i];
		min_xyz = union_min(min_xyz, t->min_xyz());
		max_xyz = union_max(max_xyz, t->max_xyz());
	}
	return AABB(min_xyz, max_xyz);
}

bool sort_x(const primitive *a, const primitive *b) {
	return a->center().x < b->center().x;
}

bool sort_y(const primitive *a, const primitive *b) {
	return a->center().y < b->center().y;
}

bool sort_z(const primitive *a, const primitive *b) {
	return a->center().z < b->center().z;
}

BVHnode* build(int l, int r, std::vector<primitive *> &scene) {
	BVHnode *node = new BVHnode();
	node->lchild = NULL;
	node->rchild = NULL;
	node->l = l;
	node->r = r;
	//std::cout << "PCNT = " << r - l + 1 << std::endl;
	//std::cout << "R = " << r << ' ' << "L = " << l << std::endl;
	if (r - l < 10) return node;
	AABB box = get_box(l, r, scene);
	//for (int i = l; i <= r; i++) std::cout << scene[i]->name << std::endl;
	//out(box.min_xyz);
	//out(box.max_xyz);
	float diffx = box.max_xyz.x - box.min_xyz.x;
	float diffy = box.max_xyz.y - box.min_xyz.y;
	float diffz = box.max_xyz.z - box.min_xyz.z;
	node->box = box;
	if (diffx > std::max(diffy, diffz)) {
		std::sort(scene.begin() + l, scene.begin() + r + 1, sort_x);
		int mid = l + r >> 1;
		node->lchild = build(l, mid, scene);
		node->rchild = build(mid + 1, r, scene);
	}
	else if (diffy > std::max(diffx, diffz)) {
		std::sort(scene.begin() + l, scene.begin() + r + 1, sort_y);
		int mid = l + r >> 1;
		node->lchild = build(l, mid, scene);
		node->rchild = build(mid + 1, r, scene);
	}
	else {
		std::sort(scene.begin() + l, scene.begin() + r + 1, sort_z);
		int mid = l + r >> 1;
		node->lchild = build(l, mid, scene);
		node->rchild = build(mid + 1, r, scene);
	}
	return node;
}

bool intersect(BVHnode *root, ray &r, hit_record &rec, std::vector<primitive *> &scene) {
	if (root == NULL) return 0;
	if (root->lchild == NULL && root->rchild == NULL) {
		float minv = 0.001, maxv = 1e18;
		hit_record t_rec;
		bool hit = false;
		for (int i = root->l; i <= root->r; i++) {
			if (scene[i]->intersect(r, t_rec) && t_rec.t >= minv && t_rec.t <= maxv) {
				maxv = std::min(maxv, t_rec.t);
				rec = t_rec;
				hit = true;
			}
		}
		return hit;
	}
	if (root->box.intersect(r)) {
		hit_record t_rec1, t_rec2;
		bool left_hit = intersect(root->lchild, r, t_rec1, scene);
		bool right_hit = intersect(root->rchild, r, t_rec2, scene);
		if (left_hit && right_hit) {
			rec = t_rec1;
			if (rec.t > t_rec2.t) rec = t_rec2;
			return 1;
		}
		if (left_hit) {
			rec = t_rec1;
			return 1;
		}
		if (right_hit) {
			rec = t_rec2;
			return 1;
		}
		return 0;
	}
	return 0;
}

void destroy(BVHnode *root) {
	if (root == NULL) return;
	destroy(root->lchild);
	destroy(root->rchild);
	delete root;
}

void debug(BVHnode *root) {
	if (root == NULL) return;
	if (root->lchild == NULL && root->rchild == NULL) {
		std::cout << "LEAF node ";
	}
	else {
		std::cout << "PARENT note ";
	}
	std::cout << "¹ÜÀí£º" << root->l << ' ' << root->r << std::endl;
	std::cout << "ºÐ×Ó£º";
	out(root->box.min_xyz);
	out(root->box.max_xyz);
	debug(root->lchild);
	debug(root->rchild);
}
