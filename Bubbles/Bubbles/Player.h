#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <glm/glm.hpp>
#include "Bubble.h"


enum MOVEDIR
{
	MOVE_FORWARD = 0,
	MOVE_BACKWARD,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_NONE
};


class CPlayer : public CBubble
{
public:
	CPlayer(glm::vec3 p = glm::vec3(0.f, 0.f, 0.f));

	virtual void update(double dt, glm::vec3 cameramMoveDir);
	virtual void move(MOVEDIR dir, bool val);
	virtual bool checkCollision(CBubble& const b);
	virtual glm::vec3 getVelocity() const;

private:
	const float m_SPEED = 5.f;
	bool m_move[6];
	glm::vec3 m_velocity[6];
};

#endif