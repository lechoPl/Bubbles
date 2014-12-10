#include "Game.h"

CGame::~CGame()
{
	if (m_currentLevel != NULL)
	{
		delete m_currentLevel;
		m_currentLevel = NULL;
	}

	if (m_camera != NULL)
	{
		delete m_camera;
		m_camera = NULL;
	}
}

void CGame::init()
{
	if (m_currentLevel != NULL)
		delete m_currentLevel;

	m_currentLevel = new CAquarium(m_levelSize, m_startBubbles);
	m_points = 0;

	m_temporaryPoints = m_points;

	printfPoints();
	
	if(m_camera == NULL)
		changeCamera();
	
	m_gameState = GAME_STATE::GAME_STARTED;

	printfNewGame();
}

void CGame::update(double dt)
{
	if (m_gameState != GAME_STATE::GAME_STARTED)
		return;
	if (m_currentLevel == NULL)
		return;
	if (m_currentLevel->getState() == AQUARIUM_STATE::GAMEOVER_LOST)
	{
		m_gameState = GAME_STATE::GAME_OVER;
		printfGameOver();
		return;
	}
	if (m_currentLevel->getState() == AQUARIUM_STATE::GAMEOVER_WIN)
	{
		m_points += pointsForLevel + m_currentLevel->getPlayerPoints();
		unsigned lastNumberOfBubbles = m_currentLevel->getNumberOfBubbles();
		
		delete m_currentLevel;
		m_currentLevel = new CAquarium(m_levelSize, lastNumberOfBubbles + m_bubblesToAdd);
		return;
	}

	while (dt > 0.0)
	{
		float deltaTime = glm::min(dt, m_dtPerUpdate);
		m_currentLevel->update(deltaTime, m_camera->getHorizontalViewDirection());
		dt -= deltaTime;
	}

	if (m_temporaryPoints != m_points + m_currentLevel->getPlayerPoints())
	{
		m_temporaryPoints = m_points + m_currentLevel->getPlayerPoints();
		printfPoints();
	}

	//move camera
	if (m_cameraType != 0)
		m_camera->moveTo(m_currentLevel->getPlayer().getPos());
}

void CGame::changeCamera()
{
	if (m_currentLevel == NULL)
		return;

	if (m_camera != NULL)
	{
		delete m_camera;
		m_camera = NULL;
	}

	m_cameraType = (m_cameraType + 1) % 3;
	switch (m_cameraType)
	{
	case 0:
		m_camera = new CArcballCamera(m_cameraPos, m_currentLevel->getPos(), glm::vec3(0.f, 1.f, 0.f));
		break;
	case 1:
		m_camera = new CFPSCamera(
			m_currentLevel->getPlayer().getPos(),
			glm::vec3(0.f, 1.f, 0.f));
		break;
	case 2:
		m_camera = new CArcballCamera(
			m_currentLevel->getPlayer().getPos() - glm::vec3(10.f, 0.f, 0.f),
			m_currentLevel->getPlayer().getPos(),
			glm::vec3(0.f, 1.f, 0.f));
		break;
	default:
		break;
	}
}

void CGame::updateCamera(glm::vec2 defaultMousePos, double dt, double mousePosX, double mousePosY)
{
	double mouseDethZone = 1;

	if (mousePosX - defaultMousePos.x < -mouseDethZone)
		m_camera->rotateLeft(dt);

	if (mousePosX - defaultMousePos.x > mouseDethZone)
		m_camera->rotateRight(dt);

	if (mousePosY - defaultMousePos.y < -mouseDethZone)
		m_camera->rotateUp(dt);

	if (mousePosY - defaultMousePos.y > mouseDethZone)
		m_camera->rotateDown(dt);
}

void CGame::printfPoints()
{
	unsigned p = m_points;
	if (m_currentLevel != NULL)
		p += m_currentLevel->getPlayerPoints();

	printf("Current points: %d\n", p);
}

void CGame::printfGameOver()
{
	printf("GAME OVER\n");
}

void CGame::printfNewGame()
{
	printf("New Game\n");
}
