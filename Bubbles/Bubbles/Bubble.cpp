#include "Bubble.h"

const float CBubble::minRadius = 0.1f;
const float CBubble::maxRadius = 1.f;

CBubble::CBubble(glm::vec3 p, float aquairumHight, glm::vec3 velocity)
	: CGameObject(p), m_aquariumHight(aquairumHight),
	m_basePosY(p.y), m_velocity(velocity)
{
	m_baseRadius = m_radius = CBubble::minRadius + 
		(CBubble::maxRadius - CBubble::minRadius) *  ((rand() % 101) / 100.f);
}

void CBubble::update(double dt)
{
	glm::vec3 tempVelocity = m_velocity;
	tempVelocity *= dt;

	m_pos += tempVelocity;

	m_radius = m_baseRadius + (m_baseRadius * 0.1f * (m_pos.y - m_basePosY) / m_aquariumHight);
}
