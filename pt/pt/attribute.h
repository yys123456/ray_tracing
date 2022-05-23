#include "material.h"
struct attribute {
	attribute(){}
	attribute(material *mat) :mat(mat) {}
	material *mat;
};