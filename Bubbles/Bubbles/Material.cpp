#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

void setMaterialUniform(GLuint programID, SMaterial& m)
{
	GLuint locID;
	locID = glGetUniformLocation(programID, "material.color");
	glUniform3fv(locID, 1, glm::value_ptr(m.color));

	locID = glGetUniformLocation(programID, "material.shininess");
	glUniform1f(locID, m.shininess);

	locID = glGetUniformLocation(programID, "material.isLightSource");
	glUniform1i(locID, m.isLightSource);

	locID = glGetUniformLocation(programID, "material.ambientReflection");
	glUniform1f(locID, m.ambientReflection);

	locID = glGetUniformLocation(programID, "material.diffuseReflection");
	glUniform1f(locID, m.diffuseReflection);

	locID = glGetUniformLocation(programID, "material.specularReflection");
	glUniform1f(locID, m.specularReflection);
};

SMaterial g_material_aquarium(
	glm::vec3(0.f, 1.f, 0.0f),
	70, 0.3, 0.2, 0.5
	);

SMaterial g_material_bubble(
	glm::vec3(0.f, 0.8f, 1.f),
	70, 0.25, 0.5, 0.3
	);

SMaterial g_material_special_bubble(
	glm::vec3(1.f, 1.f, 0.f),
	50, 0.0, 0.25, 0.25,
	true
	);

SMaterial g_material_player(
	glm::vec3(1.f, 0.2f, 0.f),
	5, 0.0, 0.25, 0.05,
	true
	);