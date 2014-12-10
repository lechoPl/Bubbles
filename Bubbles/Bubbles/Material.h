#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

struct SMaterial {
	glm::vec3 color;
	float shininess;
	bool isLightSource;
	float ambientReflection;
	float diffuseReflection;
	float specularReflection;

	SMaterial(glm::vec3 c = glm::vec3(0.f),
		float s = 0.f, float ar = 1.f,
		float dr = 1.f, float sr = 1.f,
		bool l = false) :
		color(c), shininess(s), isLightSource(l),
		ambientReflection(ar), diffuseReflection(dr),
		specularReflection(sr) { }
};

void setMaterialUniform(GLuint programID, SMaterial& m);

extern SMaterial g_material_aquarium;
extern SMaterial g_material_bubble;
extern SMaterial g_material_special_bubble;
extern SMaterial g_material_player;

#endif