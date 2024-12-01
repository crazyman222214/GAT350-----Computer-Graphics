#pragma once
#include "SceneObject.h"

class Sphere : public SceneObject
{
public:
	Sphere() = default;
	Sphere(std::shared_ptr<Material> material, float radius, const Transform& transform) : SceneObject(material, transform), m_radius{ radius } {}
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : SceneObject{ material }, m_center{ center }, m_radius{ radius } {}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;
	static bool Raycast(const ray_t& ray, glm::vec3& center, float radius, float minDistance, float maxDistance, float& t);
private:
	glm::vec3 m_center{ 0 };
	float m_radius = 0;
};