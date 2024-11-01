#pragma once
#include "Color.h"
#include "SceneObject.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

using Vertex_t = glm::vec3;
using Vertices_t = std::vector<Vertex_t>;

class Model : public SceneObject
{
public:
	Model(std::shared_ptr<Material> material) : SceneObject{ material } {}
	Model(const Vertices_t& vertices, std::shared_ptr<Material> material) : SceneObject{ material }, m_vertices{ vertices } {}


	void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera);
	bool Load(const std::string& filename);
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

private:
	Vertices_t m_vertices;
};