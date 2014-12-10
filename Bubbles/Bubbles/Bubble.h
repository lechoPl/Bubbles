#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include <glm/glm.hpp>
#include "GameObjects.h"

class CBubble : public CGameObject
{
public:
	CBubble(glm::vec3 p = glm::vec3(0.f, 0.f, 0.f),
		float aquairumHight = 0.0f,
		glm::vec3  velocity = glm::vec3(0.f, 0.f, 0.f));
	virtual void update(double dt);
	virtual float getRadius() const { return m_radius; }

	static const float minRadius;
	static const float maxRadius;

protected:
	float m_radius;
	glm::vec3 m_velocity;

private:
	const float m_basePosY;
	const float m_aquariumHight;
	float m_baseRadius;
};

#endif