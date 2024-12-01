#pragma once
#include "SceneObject.h"
#include <glm/glm.hpp>

class Plane : public SceneObject
{
public:
	Plane() = default;
	Plane(std::shared_ptr<Material> material, const Transform& transform) : SceneObject(material, transform) {}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

	static bool Raycast(const ray_t& ray, const glm::vec3& point, const glm::vec3& normal, float minDistance, float maxDistance, float& t);
};