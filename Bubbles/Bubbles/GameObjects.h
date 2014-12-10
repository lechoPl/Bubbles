#ifndef __GAMEOBJECTS__
#define __GAMEOBJECTS__

#include <glm/glm.hpp>
#include <vector>

class CGameObject
{
public:
	CGameObject(glm::vec3 p = glm::vec3(0.f, 0.f, 0.f)) { m_pos = p; }
	
	virtual glm::vec3 getPos() const { return m_pos; }
	virtual void setPos(glm::vec3 p) { m_pos = p; }

	virtual void update(double dt) = 0;

protected:
	glm::vec3 m_pos;
};

#endif