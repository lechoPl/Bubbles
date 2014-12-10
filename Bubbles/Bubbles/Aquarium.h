#ifndef __AQUARIUM_H__
#define __AQUARIUM_H__

#include <glm/glm.hpp>
#include <vector>
#include "GameObjects.h"
#include "Bubble.h"
#include "BubbleFactory.h"
#include "Player.h"

enum AQUARIUM_STATE
{
	MOVE,
	GAMEOVER_WIN,
	GAMEOVER_LOST,
};

class CAquarium : public CGameObject
{
public:
	CAquarium(glm::vec3 size = glm::vec3(1.f, 1.f, 1.f), // width, heigth, depth
		unsigned bubblesNumber = 0,
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f));
	virtual ~CAquarium();

	CPlayer& getPlayer() { return m_player; }
	std::vector<CBubble*>& getBubbles() { return m_bubbles; }
	std::vector<CBubble*>& getSpecialBubbles() { return m_specialBubbles; }
	
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }
	float getDepth() const { return m_depth; }
	
	AQUARIUM_STATE	getState() const { return m_state; }
	unsigned getNumberOfBubbles() const { return m_bubbles.size(); }
	unsigned getNumberOfSpecialBubbles() const { return m_specialBubbles.size(); }
	unsigned getPlayerPoints() const { return m_playerPoints; }

	void update(double dt) { update(dt, glm::vec3(1.f, 0.f, 0.f)); };
	void update(double dt, glm::vec3 cameramMoveDir);

public:
	static const unsigned MAX_SPECIAL_BUBBLES = 15;
protected:
	float m_width;
	float m_height;
	float m_depth;

	unsigned m_playerPoints = 0;
	const unsigned m_pointsForSpecialBubbles = 10;

	AQUARIUM_STATE m_state = AQUARIUM_STATE::MOVE;

	CPlayer	m_player;
	CBubbleFactory* m_bubbleFactory;
	std::vector<CBubble*> m_bubbles;
	std::vector<CBubble*> m_specialBubbles;
};

#endif