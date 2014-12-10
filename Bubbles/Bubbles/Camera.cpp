#include "Camera.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

const float ROTATION_SPEED = 500.f;
const float MAX_VERTICAL_ANGLE = 170.f;
const float MIN_VERTICAL_ANGLE = 10.f;
const float MAX_ZOOM = 90.f;
const float MIN_ZOOM = 1.f;

void CCamera::zoomOut()
{
	if (m_zoom >= MAX_ZOOM)
		return;

	m_zoom += 1;
}

void CCamera::zoomIn()
{
	if (m_zoom <= MIN_ZOOM)
		return;

	m_zoom -= 1;
}

void CArcballCamera::moveTo(glm::vec3 p)
{
	glm::vec3 temp = p - m_lookAt;
	m_pos += temp;
	m_lookAt += temp;
}


void CArcballCamera::rotateUp(double dt)
{
	glm::vec3 tempVec = m_pos - m_lookAt;
	glm::vec3 crossProd = glm::cross(glm::normalize(tempVec), m_up);

	tempVec = glm::rotate(tempVec, -static_cast<float>(ROTATION_SPEED * dt), crossProd);
	float angle = glm::angle(glm::normalize(tempVec), m_up);
	if (angle > MAX_VERTICAL_ANGLE)
		return;

	m_pos = m_lookAt + tempVec;
}

void CArcballCamera::rotateDown(double dt)
{
	glm::vec3 tempVec = m_pos - m_lookAt;
	glm::vec3 crossProd = glm::cross(glm::normalize(tempVec), m_up);

	tempVec = glm::rotate(tempVec, static_cast<float>(ROTATION_SPEED * dt), crossProd);
	float angle = glm::angle(glm::normalize(tempVec), m_up);
	if (angle < MIN_VERTICAL_ANGLE)
		return;

	m_pos = m_lookAt + tempVec;
}

void CArcballCamera::rotateLeft(double dt)
{
	glm::vec3 tempVec = m_pos - m_lookAt;
	m_pos = m_lookAt + glm::rotate(tempVec, static_cast<float>(ROTATION_SPEED * dt), m_up);
}

void CArcballCamera::rotateRight(double dt)
{
	glm::vec3 tempVec = m_pos - m_lookAt;
	m_pos = m_lookAt + glm::rotate(tempVec, -static_cast<float>(ROTATION_SPEED * dt), m_up);
}

glm::vec3 CArcballCamera::getHorizontalViewDirection()
{
	glm::vec3 tempVec = m_lookAt - m_pos;
	tempVec.y = 0.f;

	return glm::normalize(tempVec);
}

//-----------------

void CFPSCamera::rotateUp(double dt)
{
	glm::vec3 crossProd = glm::cross(glm::normalize(m_direction), m_up);

	glm::vec3 tempVec = glm::rotate(m_direction, static_cast<float>(ROTATION_SPEED * dt), crossProd);
	float angle = glm::angle(glm::normalize(tempVec), m_up);
	if (angle < MIN_VERTICAL_ANGLE)
		return;

	m_direction = tempVec;
}

void CFPSCamera::rotateDown(double dt)
{
	glm::vec3 crossProd = glm::cross(glm::normalize(m_direction), m_up);

	glm::vec3 tempVec = glm::rotate(m_direction, -static_cast<float>(ROTATION_SPEED * dt), crossProd);
	float angle = glm::angle(glm::normalize(tempVec), m_up);
	if (angle > MAX_VERTICAL_ANGLE)
		return;

	m_direction = tempVec;
}

void CFPSCamera::rotateLeft(double dt)
{
	m_direction = glm::rotate(m_direction, static_cast<float>(ROTATION_SPEED * dt), m_up);

}

void CFPSCamera::rotateRight(double dt)
{
	m_direction = glm::rotate(m_direction, -static_cast<float>(ROTATION_SPEED * dt), m_up);
}

glm::mat4 CFPSCamera::getViewMatrix()
{
	return glm::lookAt(m_pos, m_pos + m_direction, m_up);
}

void CFPSCamera::moveTo(glm::vec3 p)
{
	m_pos = p;
}

glm::vec3 CFPSCamera::getHorizontalViewDirection()
{
	glm::vec3 tempVec = m_direction;
	tempVec.y = 0.f;

	return glm::normalize(tempVec);
}




