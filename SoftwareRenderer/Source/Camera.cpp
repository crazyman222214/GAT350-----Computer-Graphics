#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
	m_view = glm::lookAt(eye, target, up);
}

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

/// <summary>
/// Converts point from world space to view space
/// </summary>
/// <param name="position">The given position in world space</param>
/// <returns>The given position in view (Camera) space</returns>
glm::vec3 Camera::ModelToView(const glm::vec3 position) const
{
	
	return m_view * glm::vec4{ position, 1 };
}

glm::vec4 Camera::ViewToProjection(const glm::vec3 position) const
{
	return m_projection * glm::vec4{ position, 1 };
}

glm::ivec2 Camera::ViewToScreen(const glm::vec3& position) const
{
	// convert point from view space to clip space (projection)
	glm::vec4 clip = ViewToProjection(position);
	// prevent / 0
	if (clip.w == 0) return glm::ivec2{ -1, -1 };

	// convert projection space to ndc (-1 <-> 1)
	glm::vec3 ndc = clip / clip.w;
	// don't draw if outside near and far
	if (ndc.z < -1 || ndc.z > 1) return glm::ivec2{ -1, -1 };

	float x = (ndc.x + 1) * (m_width * 0.5f);
	float y = (1 - ndc.y) * (m_height * 0.5f);

	return glm::ivec2(x, y);
}