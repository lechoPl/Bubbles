#include "Light.h"

#include <sstream>
#include <glm/gtc/type_ptr.hpp>

void setLightUniform(GLuint programID, std::vector<SLight>& lights)
{
	GLuint locID;
	locID = glGetUniformLocation(programID, "numLights");
	glUniform1i(locID, lights.size());

	std::ostringstream ss;
	for (unsigned i = 0; i < lights.size(); ++i)
	{
		ss.str("");
		ss << "allLights[" << i << "].position";
		locID = glGetUniformLocation(programID, ss.str().c_str());
		glUniform4fv(locID, 1, glm::value_ptr(lights[i].position));

		ss.str("");
		ss << "allLights[" << i << "].intensities";
		locID = glGetUniformLocation(programID, ss.str().c_str());
		glUniform3fv(locID, 1, glm::value_ptr(lights[i].intensities));

		ss.str("");
		ss << "allLights[" << i << "].power";
		locID = glGetUniformLocation(programID, ss.str().c_str());
		glUniform1f(locID, lights[i].power);
	}
}

std::vector<SLight> genereateLights(CAquarium& a)
{
	std::vector<SLight> lights;

	//global light
	lights.push_back(SLight(
		glm::vec4(3.0f, 5.0f, 7.0f, 0.0f),
		glm::vec3(1, 1, 1),
		0.8f)
		);

	//player light
	CPlayer& p = a.getPlayer();
	lights.push_back(SLight(
		glm::vec4(p.getPos(), 1.0),
		glm::vec3(1.f, 0.f, 0.f),
		10.f)
		);

	//special bubbles lights
	for (unsigned i = 0; i < a.getSpecialBubbles().size(); ++i)
	{
		CBubble* b = a.getSpecialBubbles()[i];
		if (b == NULL)
			continue;
		lights.push_back(SLight(
			glm::vec4(b->getPos(), 1.0),
			glm::vec3(0.8f, 0.8f, 0.0f) * b->getRadius() / CBubble::maxRadius,
			10.f * b->getRadius()/CBubble::maxRadius)
			);
	}

	return lights;
}

