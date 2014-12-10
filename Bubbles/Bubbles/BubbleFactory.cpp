#include "BubbleFactory.h"
#include <ctime>

CBubbleFactory::CBubbleFactory(glm::vec3 size, glm::vec3 pos, float offset, float minSpeed, float maxSpeed)
	: m_minX(pos.x - size.x / 2.f + offset + CBubble::maxRadius), m_maxX(pos.x + size.x / 2.f),
	m_minZ(pos.z - size.z / 2.f), m_maxZ(pos.z + size.z / 2.f), m_y(pos.y - size.y / 2.f),
	m_height(size.y), m_minSpeed(minSpeed), m_maxSpeed(maxSpeed)
{
	srand(static_cast<unsigned>(time(NULL)));
}

CBubble* CBubbleFactory::getBubble()
{
	float vUp = m_minSpeed < m_maxSpeed ? m_minSpeed +
		(m_maxSpeed - m_minSpeed) * ((rand() % 101) / 100.f) : 0.f;
	glm::vec3 v = glm::vec3(0.f, vUp, 0.f);

	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	if (m_minX < m_maxX && m_minZ < m_maxZ)
		pos = glm::vec3(
			m_minX + (m_maxX - m_minX) * ((rand() % 101) / 100.f),
			m_y,
			m_minZ + (m_maxZ - m_minZ) * ((rand() % 101) / 100.f)
		);

	return new CBubble(pos, m_height, v);
}