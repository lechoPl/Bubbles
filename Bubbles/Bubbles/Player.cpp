#include "Player.h"

#include <glm/gtx/vector_angle.hpp>

CPlayer::CPlayer(glm::vec3 p)
{
	for (int i = 0; i < 6; ++i)
		m_move[i] = false;

	const float val = 1.f;
	m_velocity[MOVE_FORWARD] = glm::vec3(val, 0.f, 0.f);
	m_velocity[MOVE_BACKWARD] = glm::vec3(-val, 0.f, 0.f);
	m_velocity[MOVE_RIGHT] = glm::vec3(0.f, 0.f, val);
	m_velocity[MOVE_LEFT] = glm::vec3(0.f, 0.f, -val);
	m_velocity[MOVE_UP] = glm::vec3(0.f, val, 0.f);
	m_velocity[MOVE_DOWN] = glm::vec3(0.f, -val, 0.f);

	m_pos = p;
	m_radius = 0.5f;
}

void CPlayer::update(double dt, glm::vec3 cameramMoveDir)
{
	glm::vec3 tempV = getVelocity();
	tempV *= dt;

	float angle = glm::angle(cameramMoveDir, m_velocity[MOVE_FORWARD]);
	if (angle != 0)
	{
		if (cameramMoveDir.z > 0)
			angle *= -1;

		tempV = glm::rotateY(tempV, angle);
	}

	m_pos += tempV;
}

void CPlayer::move(MOVEDIR dir, bool val)
{
	m_move[dir] = val;
}


bool CPlayer::checkCollision(CBubble& const b)
{
	float d = glm::distance(m_pos, b.getPos());

	return d < m_radius + b.getRadius();
}

glm::vec3 CPlayer::getVelocity() const
{
	glm::vec3 tempV = glm::vec3(0.f, 0.f, 0.f);
	
	for (int i = 0; i < 6; ++i)
	{
		if (m_move[i])
		{
			tempV += m_velocity[i];
		}
	}

	if (tempV.x != 0.f || tempV.y != 0.f || tempV.z != 0.f)
	{
		tempV = glm::normalize(tempV);
		tempV *= m_SPEED;
	}

	return tempV;
}
