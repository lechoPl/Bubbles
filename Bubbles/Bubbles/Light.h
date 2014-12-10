#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Aquarium.h"

struct SLight {
	glm::vec4 position;
	glm::vec3 intensities;
	float power;

	SLight(glm::vec4 pos = glm::vec4(0.f), glm::vec3 intens = glm::vec3(0.f),
		float pow = 0.f) :
		position(pos), intensities(intens), power(pow) { }
};

void setLightUniform(GLuint programID, std::vector<SLight>& lights);

std::vector<SLight> genereateLights(CAquarium& a);

#endif