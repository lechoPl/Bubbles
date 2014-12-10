#ifndef __BUBBLE_FACTORY_H__
#define __BUBBLE_FACTORY_H__

#include <glm/glm.hpp>
#include "Bubble.h"

class CBubbleFactory
{
public:
	CBubbleFactory(glm::vec3 size = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f),
		float offset = 0.f,
		float minSpeed = 0.f,
		float maxSpeed = 0.f);

	virtual CBubble* getBubble();

private:
	const float m_minX;
	const float m_maxX;
	const float m_minZ;
	const float m_maxZ;
	const float m_y; // is not random
	const float m_height;
	const float m_minSpeed;
	const float m_maxSpeed;
};

#endif