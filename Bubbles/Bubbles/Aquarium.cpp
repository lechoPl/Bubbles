#include "Aquarium.h"

CAquarium::CAquarium(glm::vec3 size, unsigned bubblesNumber, glm::vec3 pos) :
	CGameObject(pos), m_width(size.x), m_height(size.y), m_depth(size.z)
{
	//init player
	glm::vec3 playerStartPos(m_pos.x - (m_width / 2.f) + m_player.getRadius(), m_pos.y, m_pos.z);
	m_player.setPos(playerStartPos);

	//init bubble factory
	float minBubbleSpeed = 1.f + 3.f * (bubblesNumber < 50 ? bubblesNumber : 50) / 50.f;
	float maxBubbleSpeed = minBubbleSpeed + 5.f * (bubblesNumber < 50 ? bubblesNumber : 50) / 50.f;
	m_bubbleFactory = new CBubbleFactory(size, pos, 2 * m_player.getRadius(), minBubbleSpeed, maxBubbleSpeed);

	for (unsigned i = 0; i < bubblesNumber; ++i)
	{
		m_bubbles.push_back(m_bubbleFactory->getBubble());
	}

	unsigned numberOfSpecialBubbles = bubblesNumber * 0.1;
	if (numberOfSpecialBubbles > CAquarium::MAX_SPECIAL_BUBBLES)
		numberOfSpecialBubbles = CAquarium::MAX_SPECIAL_BUBBLES;

	for (unsigned i = 0; i < numberOfSpecialBubbles; ++i)
	{
		m_specialBubbles.push_back(m_bubbleFactory->getBubble());
	}
}


CAquarium::~CAquarium()
{
	for (unsigned i = 0; i < m_bubbles.size(); ++i)
	{
		if (m_bubbles.at(i) != NULL)
		{
			delete m_bubbles.at(i);
			m_bubbles[i] = NULL;
		}
	}

	for (unsigned i = 0; i < m_specialBubbles.size(); ++i)
	{
		if (m_specialBubbles[i] != NULL)
		{
			delete m_specialBubbles[i];
			m_specialBubbles[i] = NULL;
		}
	}

	if (m_bubbleFactory != NULL)
	{
		delete m_bubbleFactory;
		m_bubbleFactory = NULL;
	}
}


void CAquarium::update(double dt, glm::vec3 cameramMoveDir)
{
	if (m_state != AQUARIUM_STATE::MOVE)
		return;

	m_player.update(dt, cameramMoveDir);

	//chcek collision with walls
	glm::vec3 playerPos = m_player.getPos();
	float playerR = m_player.getRadius();

	float halfH = m_height / 2.f;
	float halfW = m_width / 2.f;
	float halfD = m_depth / 2.f;

	if (playerPos.y + playerR > m_pos.y + halfH)
		playerPos.y = m_pos.y + halfH - playerR;

	if (playerPos.y - playerR < m_pos.y - halfH)
		playerPos.y = m_pos.y - halfH + playerR;

	if (playerPos.x + playerR > m_pos.x + halfW)
	{
		playerPos.x = m_pos.x + halfW - playerR;

		m_player.setPos(playerPos);
		m_state = AQUARIUM_STATE::GAMEOVER_WIN;
		return;
	}

	if (playerPos.x - playerR < m_pos.x - halfW)
		playerPos.x = m_pos.x - halfW + playerR;

	if (playerPos.z + playerR > m_pos.z + halfD)
		playerPos.z = m_pos.z + halfD - playerR;

	if (playerPos.z - playerR < m_pos.z - halfD)
		playerPos.z = m_pos.z - halfD + playerR;

	m_player.setPos(playerPos);


	float maxHeight = halfH + m_pos.y;
	for (unsigned i = 0; i < m_bubbles.size(); ++i)
	{
		if (m_bubbles[i] == NULL)
			continue;

		if (m_player.checkCollision(*m_bubbles[i]))
		{
			m_state = AQUARIUM_STATE::GAMEOVER_LOST;
			return;
		}

		m_bubbles[i]->update(dt);

		if (m_bubbles[i]->getPos().y > maxHeight)
		{
			delete m_bubbles[i];
			m_bubbles[i] = m_bubbleFactory->getBubble();
		}
	}

	for (unsigned i = 0; i < m_specialBubbles.size(); ++i)
	{
		if (m_specialBubbles[i] == NULL)
			continue;

		if (m_player.checkCollision(*m_specialBubbles[i]))
		{
			m_playerPoints += m_pointsForSpecialBubbles;
			
			delete m_specialBubbles[i];
			m_specialBubbles[i] = NULL;

			continue;
		}

		m_specialBubbles[i]->update(dt);
		if (m_specialBubbles[i]->getPos().y > maxHeight)
		{
			delete m_specialBubbles[i];
			m_specialBubbles[i] = m_bubbleFactory->getBubble();
		}
	}

}