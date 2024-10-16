#pragma once
#include "Color.h"

#include <glm/glm.hpp>
#include <vector>

using Vertex_t = glm::vec3;
using Verticies_t = std::vector<Vertex_t>;

class Model
{
public:
	Model() = default;
	Model(const Verticies_t& verticies, const color_t& color) : m_verticies{ verticies }, m_color{ color } {}

	void Draw(class Framebuffer& framebuffer, const glm::mat4& model);

private:
	Verticies_t m_verticies;
	color_t m_color;
};