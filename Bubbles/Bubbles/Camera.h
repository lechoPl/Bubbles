#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CCamera
{
public:
	CCamera(glm::vec3 pos = glm::vec3(0.f, 0.f, 5.f),
		glm::vec3 lookAt = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f)) : m_pos(pos), m_up(up), m_lookAt(lookAt) { }
	
	virtual glm::mat4 getViewMatrix() { return glm::lookAt(m_pos, m_lookAt, m_up); }
	virtual glm::mat4 getProjectionMatix() { return glm::perspective(m_zoom, 4.0f / 3.0f, 0.1f, 100.0f); }

	virtual void rotateUp(double dt) = 0;
	virtual void rotateDown(double dt) = 0;
	virtual void rotateLeft(double dt) = 0;
	virtual void rotateRight(double dt) = 0;

	virtual glm::vec3 getHorizontalViewDirection() = 0;
	
	virtual void moveTo(glm::vec3 p) = 0;
	
	virtual void zoomIn();
	virtual void zoomOut();

	glm::vec3 getPos() const { return m_pos; }

protected:
	glm::vec3 m_pos;
	glm::vec3 m_up;
	glm::vec3 m_lookAt;

	float m_zoom = 45.f;
};

class CArcballCamera : public CCamera
{
public:
	CArcballCamera(glm::vec3 pos = glm::vec3(0.f, 0.f, 5.f),
		glm::vec3 lookAt = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f)) : CCamera(pos, lookAt, up) { }

	void rotateUp(double dt);
	void rotateDown(double dt);
	void rotateLeft(double dt);
	void rotateRight(double dt);

	glm::vec3 getHorizontalViewDirection();

	void moveTo(glm::vec3 p);
};

class CFPSCamera : public CCamera
{
public:
	CFPSCamera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f)) : CCamera(pos, up) {
		
		m_direction = glm::vec3(1.f, 0.f, 0.f);
	}

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatix() { return glm::perspective(m_zoom, 4.0f / 3.0f, 0.1f, 100.0f); }

	void rotateUp(double dt);
	void rotateDown(double dt);
	void rotateLeft(double dt);
	void rotateRight(double dt);

	glm::vec3 getHorizontalViewDirection();

	void moveTo(glm::vec3 p);

protected:
	float m_radius;
	glm::vec3 m_direction;
};

#endif