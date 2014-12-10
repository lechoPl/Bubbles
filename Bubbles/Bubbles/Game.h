#ifndef __GAME__
#define __GAME__

#include <vector>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Aquarium.h"

enum GAME_STATE
{
	GAME_NOTSTARTED,
	GAME_STARTED,
	GAME_OVER
};

class CGame
{
public:
	virtual ~CGame();
	void init();

	CAquarium* getCurrentAquarium() { return m_currentLevel; }
	CCamera* getCamera() { return m_camera; }

	void changeCamera();

	void update(double dt);
	void updateCamera(glm::vec2 defaultMousePos, double dt, double mousePosX, double mousePosY);

protected:
	virtual void printfPoints();
	virtual void printfGameOver();
	virtual void printfNewGame();

protected:
	CCamera* m_camera = NULL;
	CAquarium* m_currentLevel = NULL;
	unsigned m_points = 0; //collected point from previous level

	GAME_STATE m_gameState = GAME_STATE::GAME_NOTSTARTED;

	//consts
protected:
	const unsigned m_startBubbles = 10; //the initial number of bubbles in first level
	const unsigned m_bubblesToAdd = 5; //number of bubbles to add in next level
	const glm::vec3 m_levelSize = glm::vec3(20.f, 10.f, 10.f);

	const unsigned pointsForLevel = 100;
	const double m_dtPerUpdate = 1 / 60.0;

private:
	unsigned m_temporaryPoints; // all points
	unsigned m_cameraType = 1;

private:
	const glm::vec3 m_cameraPos = glm::vec3(0.f, 0.f, 25.f);
};

#endif